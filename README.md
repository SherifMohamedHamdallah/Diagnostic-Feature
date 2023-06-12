# Diagnostic-Feature
Diagnostic feature implemented inside a client node to read some DIDs from the server every 1s.

* Feature Specs:
● Diagnostics feature is to be implemented on a client node to be integrated inside the
vehicle..
● Diagnostics feature should follow UDS (no specific version).
● Diagnostics feature is required to read the following IDs from the server:
	○ 0x2030: 10 bytes size.
	○ 0x2031: 5 bytes size.
	○ 0x2032: 4 bytes size.
● The diagnostic IDs should be read every 1s.
● Diagnostics client CAN message ID is 0x740.
● Diagnostics server CAN message ID is 0x741.
● CAN speed is 512 Kbit/s.
● CAN Rx buffer size: 10 messages.
● Assume the UDS timing parameters.
● Assume no operating system and no other applications are running, so the main
function is totally under the responsibility of the diagnostic feature.



