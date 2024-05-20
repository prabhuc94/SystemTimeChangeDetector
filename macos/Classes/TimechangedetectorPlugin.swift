import Cocoa
import FlutterMacOS

public class TimechangedetectorPlugin: NSObject, FlutterPlugin {
  public static func register(with registrar: FlutterPluginRegistrar) {
    let channel = FlutterMethodChannel(name: "timechangedetector", binaryMessenger: registrar.messenger)
    let instance = TimechangedetectorPlugin()
    registrar.addMethodCallDelegate(instance, channel: channel)
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    switch call.method {
    case "getPlatformVersion":
      result("macOS " + ProcessInfo.processInfo.operatingSystemVersionString)
	  case "startListening":
	   DispatchQueue.global().async {
                self.setupTimeChangeListener()
                DispatchQueue.main.async {
                    result("startListening invoked")
                }
            }
    default:
      result(FlutterMethodNotImplemented)
    }
  }
  
  private func setupTimeChangeListener() {
        // Your code to set up a listener for time change events goes here
        // You can use NotificationCenter to observe system time changes
        NotificationCenter.default.addObserver(self, selector: #selector(timeChanged), name: NSNotification.Name.NSSystemClockDidChange, object: nil)
    }

    @objc private func timeChanged() {
        let channel = FlutterMethodChannel(name: "timechangedetector", binaryMessenger: FlutterMethodChannel(binaryMessenger: NSObject()))
		channel.invokeMethod("timeChanged", arguments: "Time has changed")
    }
}
