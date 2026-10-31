# Security

This is a local hardware prototype, intended for its own isolated Wi-Fi access
point. It has no application authentication, TLS, OTA updater, cloud connection,
external scripts or telemetry. Anyone connected to the AP can read and change
settings. Do not expose port 80 to the internet or bridge the AP into an untrusted
network.

`lithophane123` is a public example Wi-Fi password, not a private credential.
Replace `AP_PASSWORD` with a unique 8–63 character password before deployment.
Keep customized credentials out of Git history and shared firmware binaries.

Writes require POST, the `X-Lithophane-Control: 1` header and the AP IP as Host.
No permissive CORS is enabled. This limits ordinary browser cross-site writes and
DNS-rebinding writes; it does not authenticate an AP client. `/state` is read-only
and unauthenticated. Physical access and hostile AP clients are outside this
prototype's protection.

Settings are written directly to LittleFS. Interrupted writes can lose saved
settings. API success means accepted in RAM, not persisted to flash. See
`docs/REVIEW.md` for remaining findings and `docs/TESTING.md` for device checks.

Report security problems privately to repository owner `nullyks` using an existing
private contact channel, or GitHub private vulnerability reporting if enabled.
Do not post passwords or exploit details in a public issue. No support SLA or
production security guarantee is provided.
