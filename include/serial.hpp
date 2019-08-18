#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <inline_asm.hpp>

enum io_port_offsets {
    data_register = 0,
    interrupt_enable_register = 1,
    baud_rate_lsb = 0,
    baud_rate_hsb = 1,
    interrupt_identification_register = 2,
    fifo_control_register = 2,
    line_control_register = 3,
    modem_control_register = 4,
    line_status_register = 5,
    modem_status_register = 6,
    scratch_register = 7
};

class Serial {
  public:
    Serial();

    char read();
    void write(char c);

    static constexpr unsigned int COM1 = 0x3f8;
};

extern Serial serial;

#endif
