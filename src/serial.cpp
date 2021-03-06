#include <serial.hpp>

Serial serial;

Serial::Serial() {
    // Disable interrupts.
    outb(Serial::COM1 + interrupt_enable_register, 0x00);

    // Setting the most significant bit of the LCR sets the DLAB bit, allowing
    // us to set the baud rate divisor. The UART's internal clock runs at 115200
    // ticks per second. The baud rate is then 115200 / divisor. We'll use a
    // divisor of 3 to get a baud rate of 38400.
    outb(Serial::COM1 + line_control_register, 0x80);
    outb(Serial::COM1 + baud_rate_lsb, 0x03);
    outb(Serial::COM1 + baud_rate_hsb, 0x00);

    // The two least significant bits of the LCR control the character length in
    // bits. Setting them both to 1 corresponds to 8 bits. Bit 2 of the LCR
    // controls the stop bits. Setting it to 0 corresponds to a stop bit of 1.
    // Bits 3, 4 and 5 control the port parity. We set bit 3 to 0, which means
    // no parity.
    outb(Serial::COM1 + line_control_register, 0b0011);

    // From https://www.sci.muni.cz/docs/pc/serport.txt:
    // "To add 16550A support to existing code... After the existing UART
    // initialization code, try to enable the FIFOs by writing to the FCR. (A
    // value of C7 hex will enable FIFO mode, clear both FIFOs, and set the
    // receive trigger level at 14 bytes)."
    // Since qemu emulates a 16550A, we'll follow the instructions.
    outb(Serial::COM1 + fifo_control_register, 0xC7);

    // Bit 0 and 1 control DTR and RTS respectively. Setting bit 3 will cause
    // any interrupts generated by the UART to be transferred to the Interrupt
    // Control Unit.
    outb(Serial::COM1 + modem_control_register, 0b1011);
}

// NOLINTNEXTLINE
char Serial::read() {
    // Bit 0 indicates Data Ready. Loop until there's data to be read.
    while ((inb(Serial::COM1 + line_status_register) & 1) == 0) {
        ;
    }

    return inb(Serial::COM1);
}

// NOLINTNEXTLINE
void Serial::write(char c) {
    // Bit 5 corresponds to Transmitter Holding Register Empty. When this is set
    // a new word can be written to the Transmitter Holding Register.
    while ((inb(Serial::COM1 + line_status_register) & 0x20) == 0) {
        ;
    }

    outb(Serial::COM1, c);
}
