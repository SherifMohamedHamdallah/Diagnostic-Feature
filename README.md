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

* Feature Software Interfaces:
● CAN component interfaces
○ boolean CAN_read(unsigned int *canId, unsigned char *canData, unsigned
char *dataSize)
■ Description: pops one CAN message from the CAN Rx buffer, and
returns its ID, data pointer and data size in the output arguments of
the function.
■ Return value: whether the pop operation is successful or not.
■ Arguments: used as outputs to return the CAN message info.
○ void CAN_send(unsigned int canId, unsigned char *canData, unsigned char
dataSize)
■ Description: sends one CAN message.
■ Return value: none.
■ Arguments: used to contain the CAN message info to be sent.
● TIM component interfaces
○ unsigned int TIM_getTimestamp(void)
■ Description: gets a free running time counter value.
■ Return value: free running time counter value in 10us (1 count =
10us).
■ Arguments: none.

