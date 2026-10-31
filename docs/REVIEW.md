# Import and review — 2026-10-31

## Provenance and scope

Source: the ZIP attached to the ChatGPT conversation **Loo GitHub projekt**,
`lithophane-light-github.zip` (10,036 bytes).

SHA-256: `ad0ee7b1ca833151ef704fbbe7f6f6ba75f7cfa59753abb73a94d93a70992f2e`

The archive contains five text files: firmware, README, two documentation files
and `.gitignore`. No Git history, licenses, hardware CAD, images, binaries,
credentials files or vendored dependencies were supplied. Archive paths were
checked for traversal and symlinks before extraction. The initial commit preserves
the supplied project, with local tools and common secret files additionally ignored.

Review covered all supplied source, the embedded HTML/JavaScript, HTTP routes,
settings parsing/persistence, animation logic, dependency use and documentation.
No formal penetration test or exhaustive dependency vulnerability audit was done.

## Findings addressed

| Finding in original | Change |
| --- | --- |
| GET `/set` could mutate settings via links or foreign pages | Writes use POST plus a non-simple custom header, without CORS; Host is restricted to the AP IP to limit DNS rebinding. GET returns 405. |
| Numeric `toInt()` silently accepted junk as zero and clamped invalid ranges; partial requests could partially apply | Decimal-only bounded parsing; malformed color/power rejected; HTTP 400 leaves the entire state unchanged. |
| User could interact before `/state` completed | Controls stay disabled until initial state loads; failure gives reload guidance. |
| Slider writes could overlap | Browser serializes writes and coalesces queued changes to the latest values. |
| Duplicate settings reset effects and schedule flash writes | Identical updates now return success without resetting animation or scheduling persistence. |
| Stale state responses could be cached | `/state` sends `Cache-Control: no-store`; browser bypasses cached state. |
| Documentation referred to LED 19 and suggested adding a version already present | Direction comment uses `LED_COUNT-1`; development notes acknowledge version 2. |
| No repeatable build or automated verification | Added pinned build instructions, four embedded-script checks and a CI workflow with pinned actions. |

## Remaining findings

**Hardware clarification after the initial review (2026-10-31):** the user confirms
that the LED strip is powered directly from a 4 × 1.5 V AA battery holder, with no
voltage regulator; NodeMCU is powered separately over USB. The previous 5 V supply
description was a recommendation, not the actual prototype wiring. The user also
confirms a common ground between the battery supply, LED strip and NodeMCU.
Battery chemistry and the exact LED revision remain unconfirmed.

**High before further powered testing — possible LED overvoltage.** Four series
1.5 V cells give 6 V nominal, not regulated 5 V. Direct connection may damage a
5 V strip; fresh-cell voltage may be higher. Disconnect this supply pending LED
identification and use a suitable regulated supply. Lower software brightness
does not lower the supply voltage. Battery-holder current capability and runtime
have not been measured. See the
[Adafruit power guide](https://learn.adafruit.com/adafruit-neopixel-uberguide/powering-neopixels).

1. **High before deployment — shared Wi-Fi password and no application authentication.**
   `AP_PASSWORD` remains the supplied example to keep setup reproducible. Customize
   it locally. Every AP client remains trusted to control the light; HTTP is plaintext.
2. **Medium — non-atomic persistence and limited failure reporting.** `saveSettings()`
   truncates the active file before writing; power loss can discard the previous
   configuration. `sscanf` checks field count/ranges but accepts trailing material
   and has no checksum. `loop()` clears `savePending` even on write failure; the UI
   only confirms RAM acceptance. LittleFS uses the core's default auto-format-on-
   mount-failure behavior. A future change should use a temporary file/rename,
   stricter file parsing, explicit mount policy and surfaced save status.
3. **Medium — hardware and thermal behavior unverified.** No software current limit
   is enforced. The theoretical 40-LED full-white load is about 2.4 A at 5 V; actual
   strip variants differ. Validate supply/wiring, heat inside the printed enclosure,
   common ground, level shifting and power integrity. The 3–4 A regulated supply
   guidance is a recommendation, not the actual AA supply or a measured electrical
   certification.
4. **Low — effect discontinuities.** Changed settings reset `phase`; its later
   wrap at 2000 can also cause jumps because effects have different periods.
   These are visual limitations, not compile failures.
5. **Licensing unresolved.** No project license was supplied or selected. Keeping
   the repository private does not establish third-party redistribution rights.
   ESP8266 core and Adafruit NeoPixel retain their upstream licenses.

## Verification results

- Original and reviewed firmware compile successfully for NodeMCU using Arduino
  CLI 1.5.1, ESP8266 core 3.1.2, Adafruit NeoPixel 1.15.5, `--warnings all`.
- Reviewed build: global RAM 30,804 / 80,192 bytes (38%); IRAM including reserved
  instruction cache 60,611 / 65,536 (92%); flash code 319,588 / 1,048,576 (30%).
  IRAM headroom is limited; recheck it when expanding the firmware.
- Four Node.js script tests passed: startup loading, startup failure, serialized
  POST/coalescing and recovery after an update failure.
- Source reviewed for secrets: only the documented example AP password was found;
  no private keys or service tokens were identified. This is not a guarantee of
  complete secret detection.
- No physical lamp, serial upload, on-device HTTP regressions, browser layout test,
  power interruption test or thermal/current measurement was performed. Those
  remain explicit checklist items in `TESTING.md`.

Reference documentation: [Arduino CLI setup](https://docs.arduino.cc/arduino-cli/getting-started/),
[ESP8266 core](https://github.com/esp8266/Arduino), and
[ESP8266 filesystem API](https://arduino-esp8266.readthedocs.io/en/stable/filesystem.html).
