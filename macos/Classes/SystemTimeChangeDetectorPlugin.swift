import Cocoa
import FlutterMacOS

public class SystemTimeChangeDetectorPlugin: NSObject, FlutterPlugin {
  private var channel: FlutterMethodChannel?
  private var fileDescriptor: CInt = -1
  private var timeChangeObserver: NSObjectProtocol?
  private var source: DispatchSourceFileSystemObject?
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "systemtimechangedetector", binaryMessenger: registrar.messenger)
    let instance = SystemTimeChangeDetectorPlugin()
    instance.channel = channel
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
  if call.method == "startListening" {
        startListening()
        result("Listening for time changes")
      } else if call.method == "getPlatformVersion" {
        result("macOS " + ProcessInfo.processInfo.operatingSystemVersionString)
      } else {
        result(FlutterMethodNotImplemented)
      }
  }

  private func startListening() {
      // Your code to start listening for time changes
      // Example: Monitor system time changes or time zone changes
      // Set up observer for system clock changes
          timeChangeObserver = DistributedNotificationCenter.default.addObserver(
            forName: NSNotification.Name.NSSystemClockDidChange,
            object: nil,
            queue: nil
          ) { [weak self] notification in
            self?.systemClockDidChange()
          }
    // Set up file monitoring for timezone changes
    setupFileMonitor()
    }

    private func setupFileMonitor() {
        fileDescriptor = open("/etc/localtime", O_EVTONLY)
        if fileDescriptor == -1 {
          perror("Unable to open /etc/localtime")
          return
        }

        source = DispatchSource.makeFileSystemObjectSource(
          fileDescriptor: fileDescriptor,
          eventMask: .write,
          queue: DispatchQueue.global()
        )

        source?.setEventHandler { [weak self] in
          self?.timezoneDidChange()
        }

        source?.setCancelHandler {
          close(self.fileDescriptor)
        }

        source?.resume()
      }

    private func systemClockDidChange() {
        print("System time has changed!")
        notifyFlutter(message: "System time has changed")
      }

    private func timezoneDidChange() {
        print("Timezone has changed!")
        notifyFlutter(message: "Timezone has changed")
      }

    private func notifyFlutter(message: String) {
        if let channel = channel {
          channel.invokeMethod("timeChanged", arguments: message)
        }
      }

    @objc private func timeChanged() {
		if let channel = self.channel {
              channel.invokeMethod("timeChanged", arguments: "System time has changed")
            }
            print("Time changed")
    }
}
