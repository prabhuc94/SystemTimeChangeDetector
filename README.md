# system_time_change_detector


This plugin allows to detect timezone / time changes via Flutter desktop apps.


## Platform Support

| Linux | macOS | Windows |
| :---: | :---: | :-----: |
|  ✔️   |  ➖   |   ✔️    |

## Quick Start

### Installation

Add this to your package's pubspec.yaml file:

```yaml
dependencies:
  system_time_change_detector: ^0.0.3
```

### Example
``` dart
// Import package
import 'package:system_time_change_detector/systemtimechangedetector.dart';

// Initialize variable
final _timechangedetectorPlugin = SystemTimeChangeDetector();
await _timechangedetectorPlugin.getSystemTimeChange(() {
        debugPrint("TIME / ZONE CHANGED"); // Handle the changes
      });
```


> Please see the example app of this plugin for a full example.

## Platform Differences

| Name     | Description                       | Linux | macOS | Windows |
| -------- |-----------------------------------| ----- | ----- | ------- |
| `timezone` | Detect system timezone changes    | ✔️    | ➖  | ✔️ |
| `time` | Detect system time / date changes | ➖    | ➖  | ✔️ |


## License

[MIT](./LICENSE)
