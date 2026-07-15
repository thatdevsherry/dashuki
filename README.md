# Dashuki

Graphical dashboard for my beloved suzuki.

Successor to [suzui-rs](https://github.com/thatdevsherry/suzui-rs).

Written in C, using [raylib](https://www.raylib.com) for the GUI.

**Note:** Still a WIP, but is already running live in my car in place of suzui-rs.

![screenshot](https://github.com/user-attachments/assets/2f2ca919-b0fd-408b-820c-c74d5f3543d9)

[simulation](https://github.com/user-attachments/assets/144321cd-6bb6-4a60-8efa-0b9354e6fc11)

![First look on YT](https://youtu.be/vMgydCYH9T4)

## Main workflow

```bash
# Set up `.env` from `.env.sample`.
# For local run, modify DATA and SETTINGS path to some dir.
# Buildroot var required only for cross-compile.
cp .env.sample .env

# Create PTY using `socat`
socat -d -d pty,raw,echo=0 pty,raw,echo=0

# Run simulator from my old prototype (has some changes to ignore baudrate on PTY)
uv run python sim.py /dev/pts/{MASTER} --echo

# Run on PC for local dev
make debug DEVICE=/dev/pts/{SLAVE}

# Run on PC with actual VAG KKL cable (e.g. ttyUSB0)
make debug DEVICE=/dev/ttyUSB0

# Cross-compile arm64 binary using buildroot cross-compiler toolchain & sysroot
make pi
```

## Phase 1 (complete)

Feature parity with suzui-rs.

- [x] Engine Speed
- [x] Desired idle
- [x] ISC flow duty
- [x] Inj. pulse width
- [x] Ignition advance
- [x] IAT
- [x] ECT
- [x] Voltage-mapped Throttle position
- [x] Throttle angle
- [x] MAP
- [x] Baro
- [x] Calc. load
- [x] Battery voltage
- [x] Vehicle speed
- [x] Flags (EL, AC, PSP, RAD)
- [x] Instant fuel use
- [x] Instant fuel average
- [x] Trip fuel used
- [x] Trip fuel average
- [x] Trip distance
- [x] Reset trip (by toggling EL a few times)

Goodies that weren't present in suzui-rs:

- [x] Odometer
- [x] Menu (keyboard/controller control)

## Menu (WIP)

- [x] Toggle diag params
- [x] Toggle fuel avg unit (km/L, L/100km)
- [x] Reset trip
- [ ] Toggle RPM/idle numbers
- [ ] DTC read
- [ ] DTC clear
- [ ] Actuate ISC
- [ ] Actuate fixed spark

### Phase 2

Hold your horses. Polish phase 1 first (animations, menu, DTCs, C code quality etc).

## AI Notice

No claude code, no `{INSERT_TRENDING_SLOP_MACHINE_HERE}`. All code is brain-generated.

AI use is limited to web chat interface. Asked during explorations and
pointers to concepts for further reading.

No "skim generated code; good to go".

AI is nice for exploration. It told me I could utilise buildroot for cross-compiling.
That's good. Adding slop is not.
