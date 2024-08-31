# Robotraffic car

## Hall sensor

| hall | arduino |
| ---- | ------- |
| GND  | GND     |
| +    | 5V      |
| D0   | 2       |

## L298n

| driver | arduino |
| ------ | ------- |
| PWMA   | 3       |
| AIN1   | 4       |
| AIN2   | 6       |

## QTRSensor 5RC

| line | arduino |
| ---- | ------- |
| 1    | 9       |
| 2    | 10      |
| 3    | 11      |
| 4    | 12      |
| 5    | 13      |

## Servo

| servo  | arduino |
| ------ | ------- |
| signal | 7       |

## All topic

- input
  - parameter: update_data
    - all: get_all
  - control
    - hall: setup
    - motor: setup
    - line: setup
    - steering: setup
    - all: setup all
    - car: change mode (test mode / real mode)
- output
  - parameter
    - PID
      - input: double
      - output: double
  - state
    - hall: 0 / 1
    - motor: 0 / 1
    - line: 0 / 1
    - steering: 0 / 1

## Control flow

```mermaid
graph TD
serial[[Serial]] --> net[[network]] --> mqtt[[mqtt]] --> setup[[Setup ]] --> mode




mode{Current mode?} --> change_mode{{Change mode}} ==> setup

mode --> |Test| hall_off[Turn off hall sensor] --> canGo{Can go?}
canGo -->|Yes| angle[update angle] --> Run --> canGo
canGo --> |No| motor_off[Stop]

mode --> |Real| hall(hall) ==> canGo

```
