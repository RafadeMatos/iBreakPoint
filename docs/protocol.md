# iBreakPoint serial protocol v1

- Transport: USB CDC serial, 115200 baud, 8 data bits, no parity, 1 stop bit.
- Encoding: ASCII command and response lines terminated by LF (`\n`); CR is ignored by firmware.
- Maximum firmware command payload: 159 bytes (excluding terminator).
- Host response buffer: 511 bytes; host timeout is 1200 ms for DocTor Who and 1000 ms for Analyze.
- Frame: `IBP/1 COMMAND [arguments]\n`.
- Response: `IBP/1 OK payload\n` or `IBP/1 ERR payload\n`.

Commands:

| Request | Success response (example) |
|---|---|
| `IBP/1 HELLO` | `IBP/1 OK device=RP2040 protocol=1` |
| `IBP/1 INFO` | `IBP/1 OK firmware=0.1.0 build=dev target=RP2040` |
| `IBP/1 CAPS` | `IBP/1 OK hello,info,caps` |

Unknown requests return `IBP/1 ERR code=BAD_COMMAND`. Overlong input is discarded and returns `IBP/1 ERR code=LINE_TOO_LONG`. The host validates the protocol prefix and response kind, handles timeout/disconnect, and does not infer firmware identity from USB IDs. The current protocol is line-based and intentionally small; it is not authenticated and should only be used with trusted devices.
