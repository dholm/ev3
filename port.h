#ifndef PORT_H
#define PORT_H

typedef enum in_port_e {
    InPort_1 = 0,
    InPort_2,
    InPort_3,
    InPort_4,
} in_port_t;

typedef enum out_port_e {
    OutPort_A = (1 << 0),
    OutPort_B = (1 << 1),
    OutPort_C = (1 << 2),
    OutPort_D = (1 << 3),
} out_port_t;

#endif /* PORT_H */
