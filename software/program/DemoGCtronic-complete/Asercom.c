#include "p30F6014A.h"
// Advanced Sercom B

#define CLIFF_SENSORS
#define FLOOR_SENSORS	// define to enable floor sensors
#define IR_RECEIVER

#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include <motor_led/e_epuck_ports.h>
#include <motor_led/e_init_port.h>
#include <motor_led/advance_one_timer/e_led.h>
#include <motor_led/advance_one_timer/e_motors.h>
#include <uart/e_uart_char.h>
#include <a_d/advance_ad_scan/e_ad_conv.h>
#include <a_d/advance_ad_scan/e_acc.h>
#include <a_d/advance_ad_scan/e_prox.h>
#include <a_d/advance_ad_scan/e_micro.h>
#include <motor_led/advance_one_timer/e_agenda.h>
#include <camera/fast_2_timer/e_po8030d.h>
#include <camera/fast_2_timer/e_poxxxx.h>
#include <codec/e_sound.h>
#include <utility/utility.h>
#include <acc_gyro/e_lsm330.h>
#ifdef CLIFF_SENSORS
#ifndef FLOOR_SENSORS
#define FLOOR_SENSORS
#endif
#endif
#ifdef FLOOR_SENSORS
#include <I2C/e_I2C_protocol.h>
#endif 

#ifdef IR_RECEIVER
#include <motor_led/advance_one_timer/e_remote_control.h>
#define SPEED_IR 600
#endif 
#include "DataEEPROM.h"

#include "memory.h"
extern char buffer[BUFFER_SIZE];
extern int e_mic_scan[3][MIC_SAMP_NB];
extern unsigned int e_last_mic_scan_id;
extern int selector;
extern char c;

#define uart1_send_static_text(msg) do { e_send_uart1_char(msg,sizeof(msg)-1); while(e_uart1_sending()); } while(0)
#define uart1_send_text(msg) do { e_send_uart1_char(msg,strlen(msg)); while(e_uart1_sending()); } while(0)
#define uart2_send_static_text(msg) do { e_send_uart2_char(msg,sizeof(msg)-1); while(e_uart2_sending()); } while(0)
#define uart2_send_text(msg) do { e_send_uart2_char(msg,strlen(msg)); while(e_uart2_sending()); } while(0)

int run_asercom(void) {
    static char c1, c2, wait_cam = 0;
    static int i, j, n, speedr, speedl, positionr, positionl, LED_nbr, LED_action, accx, accy, accz, sound, gyrox, gyroy, gyroz;
    static int cam_mode, cam_width, cam_heigth, cam_zoom, cam_size, cam_x1, cam_y1;
    static char first = 0;
    char *ptr;
    static int mod, reg, val;
#ifdef IR_RECEIVER
    char ir_move = 0, ir_address = 0, ir_last_move = 0;
#endif
    static TypeAccSpheric accelero;
    //static TypeAccRaw accelero_raw;
    int use_bt = 0;
    unsigned int battValue = 0;

    //e_init_port();    // configure port pins
    //e_start_agendas_processing();
    e_init_motors();
    //e_init_uart1();   // initialize UART to 115200 Kbaud
    //e_init_ad_scan();

    selector = getselector(); //SELECTOR0 + 2*SELECTOR1 + 4*SELECTOR2 + 8*SELECTOR3;
    if (selector == 10) {
        use_bt = 0;
    } else {
        use_bt = 1;
    }

#ifdef FLOOR_SENSORS
    if (use_bt) { // the I2C must remain disabled when using the gumstix extension
        e_i2cp_init();
    }
#endif

#ifdef IR_RECEIVER
    e_init_remote_control();
#endif
    if (RCONbits.POR) { // reset if power on (some problem for few robots)
        RCONbits.POR = 0;
        RESET();
    }
    /*read HW version from the eeprom (last word)*/
    static int HWversion = 0xFFFF;
    ReadEE(0x7F, 0xFFFE, &HWversion, 1);

    /*Cam default parameter*/
    cam_mode = RGB_565_MODE;
    //cam_mode=GREY_SCALE_MODE;
    cam_width = 40; // DEFAULT_WIDTH;
    cam_heigth = 40; // DEFAULT_HEIGHT;
    cam_zoom = 8;
    cam_size = cam_width * cam_heigth * 2;

    if (use_bt) {
        e_poxxxx_init_cam();
        e_poxxxx_config_cam((ARRAY_WIDTH - cam_width * cam_zoom) / 2, (ARRAY_HEIGHT - cam_heigth * cam_zoom) / 2, cam_width*cam_zoom, cam_heigth*cam_zoom, cam_zoom, cam_zoom, cam_mode);
        e_poxxxx_write_cam_registers();
    }

     if (use_bt) {
        e_acc_calibr();
        uart1_send_static_text("\f\a"
                "WELCOME to the SerCom protocol on e-Puck\r\n"
                "the EPFL education robot type \"H\" for help\r\n");
    } else {
         if(isEpuckVersion1_3()==0) {
             e_acc_calibr();
         }
        uart2_send_static_text("\f\a"
                "WELCOME to the SerCom protocol on e-Puck\r\n"
                "the EPFL education robot type \"H\" for help\r\n");
    }

    while (1) {
        if (use_bt) {
            while (e_getchar_uart1(&c) == 0)
#ifdef IR_RECEIVER
            {
                ir_move = e_get_data();
                ir_address = e_get_address();
                if (((ir_address == 0) || (ir_address == 8)) && (ir_move != ir_last_move)) {
                    switch (ir_move) {
                        case 1:
                            speedr = SPEED_IR;
                            speedl = SPEED_IR / 2;
                            break;
                        case 2:
                            speedr = SPEED_IR;
                            speedl = SPEED_IR;
                            break;
                        case 3:
                            speedr = SPEED_IR / 2;
                            speedl = SPEED_IR;
                            break;
                        case 4:
                            speedr = SPEED_IR;
                            speedl = -SPEED_IR;
                            break;
                        case 5:
                            speedr = 0;
                            speedl = 0;
                            break;
                        case 6:
                            speedr = -SPEED_IR;
                            speedl = SPEED_IR;
                            break;
                        case 7:
                            speedr = -SPEED_IR;
                            speedl = -SPEED_IR / 2;
                            break;
                        case 8:
                            speedr = -SPEED_IR;
                            speedl = -SPEED_IR;
                            break;
                        case 9:
                            speedr = -SPEED_IR / 2;
                            speedl = -SPEED_IR;
                            break;
                        case 0:
                            if (first == 0) {
                                e_init_sound();
                                first = 1;
                            }
                            e_play_sound(11028, 8016);
                            break;
                        default:
                            speedr = speedl = 0;
                    }
                    ir_last_move = ir_move;
                    e_set_speed_left(speedl);
                    e_set_speed_right(speedr);
                }
            }
#else 
                ;
#endif
        } else {
            while (e_getchar_uart2(&c) == 0)
#ifdef IR_RECEIVER
            {
                ir_move = e_get_data();
                ir_address = e_get_address();
                if (((ir_address == 0) || (ir_address == 8)) && (ir_move != ir_last_move)) {
                    switch (ir_move) {
                        case 1:
                            speedr = SPEED_IR;
                            speedl = SPEED_IR / 2;
                            break;
                        case 2:
                            speedr = SPEED_IR;
                            speedl = SPEED_IR;
                            break;
                        case 3:
                            speedr = SPEED_IR / 2;
                            speedl = SPEED_IR;
                            break;
                        case 4:
                            speedr = SPEED_IR;
                            speedl = -SPEED_IR;
                            break;
                        case 5:
                            speedr = 0;
                            speedl = 0;
                            break;
                        case 6:
                            speedr = -SPEED_IR;
                            speedl = SPEED_IR;
                            break;
                        case 7:
                            speedr = -SPEED_IR;
                            speedl = -SPEED_IR / 2;
                            break;
                        case 8:
                            speedr = -SPEED_IR;
                            speedl = -SPEED_IR;
                            break;
                        case 9:
                            speedr = -SPEED_IR / 2;
                            speedl = -SPEED_IR;
                            break;
                        case 0:
                            if (first == 0) {
                                e_init_sound();
                                first = 1;
                            }
                            e_play_sound(11028, 8016);
                            break;
                        default:
                            speedr = speedl = 0;
                    }
                    ir_last_move = ir_move;
                    e_set_speed_left(speedl);
                    e_set_speed_right(speedr);
                }
            }
#else 
                ;
#endif
        }

        if (c < 0) { // binary mode (big endian)
            i = 0;
            do {
                switch (-c) {
                    case 'a': // Read acceleration sensors in a non filtered way, same as ASCII
                        if(use_bt) {
                            accx = e_get_acc_filtered(0, 1);
                            accy = e_get_acc_filtered(1, 1);
                            accz = e_get_acc_filtered(2, 1);
                            //accx = e_get_acc(0);	//too much noisy
                            //accy = e_get_acc(1);
                            //accz = e_get_acc(2);
                        } else {
                            if(isEpuckVersion1_3()) {
                                accx = 0;
                                accy = 0;
                                accz = 0;
                            } else {
                                accx = e_get_acc_filtered(0, 1);
                                accy = e_get_acc_filtered(1, 1);
                                accz = e_get_acc_filtered(2, 1);
                            }
                        }
                        buffer[i++] = accx & 0xff;
                        buffer[i++] = accx >> 8;
                        buffer[i++] = accy & 0xff;
                        buffer[i++] = accy >> 8;
                        buffer[i++] = accz & 0xff;
                        buffer[i++] = accz >> 8;
                        break;

                    case 'A': // read acceleration sensors
                        if(use_bt) {
                            accelero = e_read_acc_spheric();
                        } else {
                            if(isEpuckVersion1_3()) {
                                accelero.acceleration = 0.0;
                                accelero.inclination = 0.0;
                                accelero.orientation = 0.0;
                            } else {
                                accelero = e_read_acc_spheric();
                            }
                        }
                        ptr = (char *) &accelero.acceleration;
                        buffer[i++] = (*ptr);
                        ptr++;
                        buffer[i++] = (*ptr);
                        ptr++;
                        buffer[i++] = (*ptr);
                        ptr++;
                        buffer[i++] = (*ptr);

                        ptr = (char *) &accelero.orientation;
                        buffer[i++] = (*ptr);
                        ptr++;
                        buffer[i++] = (*ptr);
                        ptr++;
                        buffer[i++] = (*ptr);
                        ptr++;
                        buffer[i++] = (*ptr);

                        ptr = (char *) &accelero.inclination;
                        buffer[i++] = (*ptr);
                        ptr++;
                        buffer[i++] = (*ptr);
                        ptr++;
                        buffer[i++] = (*ptr);
                        ptr++;
                        buffer[i++] = (*ptr);
                        break;

                    case 'b': // battery state
                        if (isEpuckVersion1_3()) {
                            battValue = getBatteryValuePercentage();
                            buffer[i++] = battValue & 0xFF;
                            buffer[i++] = battValue << 8;
                        } else {
                            buffer[i++] = BATT_LOW; // BATT_LOW=1 => battery ok, BATT_LOW=0 => battery<3.4V
                            buffer[i++] = 0; // to have a packet of 2 bytes (for consistency)
                        }
                        break;

                    case 'D': // set motor speed
                        if (use_bt) {
                            while (e_getchar_uart1(&c1) == 0);
                            while (e_getchar_uart1(&c2) == 0);
                        } else {
                            while (e_getchar_uart2(&c1) == 0);
                            while (e_getchar_uart2(&c2) == 0);
                        }
                        speedl = (unsigned char) c1 + ((unsigned int) c2 << 8);
                        if (use_bt) {
                            while (e_getchar_uart1(&c1) == 0);
                            while (e_getchar_uart1(&c2) == 0);
                        } else {
                            while (e_getchar_uart2(&c1) == 0);
                            while (e_getchar_uart2(&c2) == 0);
                        }
                        speedr = (unsigned char) c1 + ((unsigned int) c2 << 8);
                        e_set_speed_left(speedl);
                        e_set_speed_right(speedr);
                        break;
                    case 'E': // get motor speed
                        buffer[i++] = speedl & 0xff;
                        buffer[i++] = speedl >> 8;
                        buffer[i++] = speedr & 0xff;
                        buffer[i++] = speedr >> 8;
                        break;

                    case 'g': // gyro rates
                        if(use_bt && isEpuckVersion1_3()) {
                            getAllAxesGyro(&gyrox, &gyroy, &gyroz);
                            buffer[i++] = gyrox & 0xFF;
                            buffer[i++] = gyrox >> 8;
                            buffer[i++] = gyroy & 0xFF;
                            buffer[i++] = gyroy >> 8;
                            buffer[i++] = gyroz & 0xFF;
                            buffer[i++] = gyroz >> 8;
                        } else {
                            buffer[i++] = 0;
                            buffer[i++] = 0;
                            buffer[i++] = 0;
                            buffer[i++] = 0;
                            buffer[i++] = 0;
                            buffer[i++] = 0;
                        }
                        break;

                    case 'I': // get camera image
                        if (use_bt) {
                            e_poxxxx_launch_capture(&buffer[i + 3]);
                            wait_cam = 1;
                            buffer[i++] = (char) cam_mode & 0xff; //send image parameter
                            buffer[i++] = (char) cam_width & 0xff;
                            buffer[i++] = (char) cam_heigth & 0xff;
                            i += cam_size;
                        }
                        break;
                    case 'L': // set LED
                        if (use_bt) {
                            while (e_getchar_uart1(&c1) == 0);
                            while (e_getchar_uart1(&c2) == 0);
                        } else {
                            while (e_getchar_uart2(&c1) == 0);
                            while (e_getchar_uart2(&c2) == 0);
                        }
                        switch (c1) {
                            case 8:
                                if (use_bt) {
                                    e_set_body_led(c2);
                                }
                                break;
                            case 9:
                                if (use_bt) {
                                    e_set_front_led(c2);
                                }
                                break;
                            default:
                                e_set_led(c1, c2);
                                break;
                        }
                        break;
                    case 'M': // optional floor sensors
#ifdef FLOOR_SENSORS
                        if (use_bt) {
                            e_i2cp_init();
                            e_i2cp_enable();
                            e_i2cp_read(0xC0, 0);
                            for (j = 0; j < 6; j++) {
                                if (j % 2 == 0) buffer[i++] = e_i2cp_read(0xC0, j + 1);
                                else buffer[i++] = e_i2cp_read(0xC0, j - 1);
                            }
#ifdef CLIFF_SENSORS
                            for (j = 13; j < 17; j++) {
                                if (j % 2 == 0) buffer[i++] = e_i2cp_read(0xC0, j - 1);
                                else buffer[i++] = e_i2cp_read(0xC0, j + 1);
                            }
#endif
                            e_i2cp_disable();
                        }
#else
                        for (j = 0; j < 6; j++) buffer[i++] = 0;
#endif
                        break;
                    case 'N': // read proximity sensors
                        if (use_bt) {
                            for (j = 0; j < 8; j++) {
                                n = e_get_calibrated_prox(j); // or ? n=e_get_prox(j);
                                buffer[i++] = n & 0xff;
                                buffer[i++] = n >> 8;
                            }
                        } else {
                            for (j = 0; j < 10; j++) {
                                n = e_get_calibrated_prox(j); // or ? n=e_get_prox(j);
                                buffer[i++] = n & 0xff;
                                buffer[i++] = n >> 8;
                            }
                        }
                        break;
                    case 'O': // read light sensors
                        if (use_bt) {
                            for (j = 0; j < 8; j++) {

                                n = e_get_ambient_light(j);
                                buffer[i++] = n & 0xff;
                                buffer[i++] = n >> 8;
                            }
                        } else {
                            for (j = 0; j < 10; j++) {
                                n = e_get_ambient_light(j);
                                buffer[i++] = n & 0xff;
                                buffer[i++] = n >> 8;
                            }
                        }
                        break;
                        
                    case 'P': // set motor position
                        if (use_bt) {
                            while (e_getchar_uart1(&c1) == 0);
                            while (e_getchar_uart1(&c2) == 0);
                        } else {
                            while (e_getchar_uart2(&c1) == 0);
                            while (e_getchar_uart2(&c2) == 0);
                        }
                        positionl = (unsigned char) c1 + ((unsigned int) c2 << 8);
                        if (use_bt) {
                            while (e_getchar_uart1(&c1) == 0);
                            while (e_getchar_uart1(&c2) == 0);
                        } else {
                            while (e_getchar_uart2(&c1) == 0);
                            while (e_getchar_uart2(&c2) == 0);
                        }
                        positionr = (unsigned char) c1 + ((unsigned int) c2 << 8);
                        e_set_steps_left(positionl);
                        e_set_steps_right(positionr);
                        break;
                    case 'Q': // read encoders
                        n = e_get_steps_left();
                        buffer[i++] = n & 0xff;
                        buffer[i++] = n >> 8;
                        n = e_get_steps_right();
                        buffer[i++] = n & 0xff;
                        buffer[i++] = n >> 8;
                        break;

                    case 't': // temperature
                        if (use_bt && isEpuckVersion1_3()) {
                            buffer[i++] = getTemperature();
                        } else {
                            buffer[i++] = 0;
                        }
                        break;

                    case 'u': // get last micro volumes
                        n = e_get_micro_volume(0);
                        buffer[i++] = n & 0xff;
                        buffer[i++] = n >> 8;

                        n = e_get_micro_volume(1);
                        buffer[i++] = n & 0xff;
                        buffer[i++] = n >> 8;

                        n = e_get_micro_volume(2);
                        buffer[i++] = n & 0xff;
                        buffer[i++] = n >> 8;
                        break;
                    case 'U': // get micro buffer
                        ptr = (char *) e_mic_scan;
                        if (use_bt) {
                            e_send_uart1_char(ptr, 600); //send sound buffer
                        } else {
                            e_send_uart2_char(ptr, 600); //send sound buffer
                        }
                        n = e_last_mic_scan_id; //send last scan
                        buffer[i++] = n & 0xff;
                        break;
                    case 'W':
                        if (use_bt) {
                            while (e_getchar_uart1((char*)&mod)==0);
                            while (e_getchar_uart1((char*)&reg)==0);
                            while (e_getchar_uart1((char*)&val)==0);
                            e_i2cp_enable();
                            e_i2cp_write((char)mod, (char)reg, (char)val);	// write I2C
                            e_i2cp_disable();
                        }
                        break;
                    case 'w':	// RGB-panel extension command: write 9-LEDs + 8 IRs setting through I2C (RGB-panel I2C address = 176)
                        if (use_bt) {
                            e_i2cp_enable();
                            for(j=0; j<24; j++) {
                                while (e_getchar_uart1(&buffer[j])==0);
                            }
                            for(j=146; j<149; j++) {
                                while (e_getchar_uart1(&buffer[j-122])==0);
                            }
                            for(j=164; j<172; j++) {
                                while (e_getchar_uart1(&buffer[j-137])==0);
                            }
                            for(j=0; j<24; j++) {
                                e_i2cp_write((unsigned char)176, (unsigned char)j, (unsigned char)buffer[j]);
                            }
                            for(j=146; j<149; j++) {
                                e_i2cp_write((unsigned char)176, (unsigned char)j, (unsigned char)buffer[j-122]);
                            }
                            for(j=164; j<172; j++) {
                                e_i2cp_write((unsigned char)176, (unsigned char)j, (unsigned char)buffer[j-137]);
                            }
                            e_i2cp_write((unsigned char)176, (unsigned char)145, (unsigned char)1);
                            e_i2cp_disable();
                        }
			break;
                    default: // silently ignored
                        break;
                }
                if (use_bt) {
                    while (e_getchar_uart1(&c) == 0); // get next command
                } else {
                    while (e_getchar_uart2(&c) == 0); // get next command
                }
            } while (c);
            if (i != 0) {
                if (use_bt) {
                    if (wait_cam) {
                        wait_cam = 0;
                        while (!e_poxxxx_is_img_ready());
                    }
                }

                if (use_bt) {
                    e_send_uart1_char(buffer, i); // send answer
                    while (e_uart1_sending());
                } else {
                    e_send_uart2_char(buffer, i); // send answer
                    while (e_uart2_sending());
                }
            }
            // **** ascii mode ****
        } else if (c > 0) { // ascii mode
            if (use_bt) {
                while (c == '\n' || c == '\r') e_getchar_uart1(&c);
            } else {
                while (c == '\n' || c == '\r') e_getchar_uart2(&c);
            }
            buffer[0] = c;
            i = 1;
            if (use_bt) {
                do if (e_getchar_uart1(&c)) buffer[i++] = c;
                    while (c != '\n' && c != '\r');
            } else {
                do if (e_getchar_uart2(&c)) buffer[i++] = c;
                    while (c != '\n' && c != '\r');
            }
            buffer[i++] = '\0';

            if ((buffer[0] != 'b') && (buffer[0] != 'g')) {
                buffer[0] = toupper(buffer[0]); // we also accept lowercase letters
            }
            switch (buffer[0]) {
                case 'A': // read accelerometer
                    if(use_bt) {
                        sprintf(buffer, "a,%d,%d,%d\r\n", e_get_acc_filtered(0, 1), e_get_acc_filtered(1, 1), e_get_acc_filtered(2, 1));
                    } else {
                        if(isEpuckVersion1_3()) {
                            sprintf(buffer, "a,0,0,0\r\n");
                        } else {
                            sprintf(buffer, "a,%d,%d,%d\r\n", e_get_acc_filtered(0, 1), e_get_acc_filtered(1, 1), e_get_acc_filtered(2, 1));
                        }
                    }
                    if (use_bt) {
                        uart1_send_text(buffer);
                    } else {
                        uart2_send_text(buffer);
                    }
                    break;
                case 'b': // battery state
                    if (isEpuckVersion1_3()) {
                        sprintf(buffer, "b,%d (%d%%)\r\n", getBatteryValueRaw(), getBatteryValuePercentage());
                    } else {
                        sprintf(buffer, "b,%d\r\n", BATT_LOW); // BATT_LOW=1 => battery ok, BATT_LOW=0 => battery<3.4V
                    }
                    if (use_bt) {
                        uart1_send_text(buffer);
                    } else {
                        uart2_send_text(buffer);
                    }
                    break;
                case 'B': // set body led
                    sscanf(buffer, "B,%d\r", &LED_action);
                    if (use_bt) {
                        e_set_body_led(LED_action);
                        uart1_send_static_text("b\r\n");
                    } else {
                        uart2_send_static_text("b\r\n");
                    }
                    break;
                case 'C': // read selector position
                    selector = SELECTOR0 + 2 * SELECTOR1 + 4 * SELECTOR2 + 8 * SELECTOR3;
                    sprintf(buffer, "c,%d\r\n", selector);
                    if (use_bt) {
                        uart1_send_text(buffer);
                    } else {
                        uart2_send_text(buffer);
                    }
                    break;
                case 'D': // set motor speed
                    sscanf(buffer, "D,%d,%d\r", &speedl, &speedr);
                    e_set_speed_left(speedl);
                    e_set_speed_right(speedr);
                    if (use_bt) {
                        uart1_send_static_text("d\r\n");
                    } else {
                        uart2_send_static_text("d\r\n");
                    }
                    break;
                case 'E': // read motor speed
                    sprintf(buffer, "e,%d,%d\r\n", speedl, speedr);
                    if (use_bt) {
                        uart1_send_text(buffer);
                    } else {
                        uart2_send_text(buffer);
                    }
                    break;
                case 'F': // set front led
                    sscanf(buffer, "F,%d\r", &LED_action);
                    if (use_bt) {
                        e_set_front_led(LED_action);
                        uart1_send_static_text("f\r\n");
                    } else {
                        uart2_send_static_text("f\r\n");
                    }
                    break;
#ifdef IR_RECEIVER				
                case 'G':
                    if (use_bt) {
                        sprintf(buffer, "g IR check : 0x%x, address : 0x%x, data : 0x%x\r\n", e_get_check(), e_get_address(), e_get_data());
                        uart1_send_text(buffer);
                    }
                    break;
#endif
                case 'g': // gyro rates
                    if (use_bt && isEpuckVersion1_3()) {
                        sprintf(buffer, "g,%d,%d,%d\r\n", getXAxisGyro(), getYAxisGyro(), getZAxisGyro());
                    } else {
                        sprintf(buffer, "g,0,0,0\r\n");
                    }
                    if (use_bt) {
                        uart1_send_text(buffer);
                    } else {
                        uart2_send_text(buffer);
                    }
                    break;

                case 'H': // ask for help
                    if (use_bt) {
                        uart1_send_static_text("\n");
                        uart1_send_static_text("\"A\"               Accelerometer\r\n");
                        uart1_send_static_text("\"B,#\"             Body led 0=off 1=on 2=inverse\r\n");
                        if (isEpuckVersion1_3()) {
                            uart1_send_static_text("\"b\"               Battery value\r\n");
                        } else {
                            uart1_send_static_text("\"b\"               Battery state (1=ok, 0=low)\r\n");
                        }
                        uart1_send_static_text("\"C\"               Selector position\r\n");
                        uart1_send_static_text("\"D,#,#\"           Set motor speed left,right\r\n");
                        uart1_send_static_text("\"E\"               Get motor speed left,right\r\n");
                        uart1_send_static_text("\"F,#\"             Front led 0=off 1=on 2=inverse\r\n");
#ifdef IR_RECEIVER
                        uart1_send_static_text("\"G\"               IR receiver\r\n");
#endif
                        if (isEpuckVersion1_3()) {
                            uart1_send_static_text("\"g\"               Gyro\r\n");
                        }
                        uart1_send_static_text("\"H\"               Help\r\n");
                        uart1_send_static_text("\"I\"               Get camera parameter\r\n");
                        uart1_send_static_text("\"J,#,#,#,#,#,#\"   Set camera parameter mode,width,heigth,zoom(1,4 or 8),x1,y1\r\n");
                        uart1_send_static_text("\"K\"               Calibrate proximity sensors\r\n");
                        uart1_send_static_text("\"L,#,#\"           Led number,0=off 1=on 2=inverse\r\n");
#ifdef FLOOR_SENSORS
                        uart1_send_static_text("\"M\"               Floor sensors\r\n");
#endif
                        uart1_send_static_text("\"N\"               Proximity\r\n");
                        uart1_send_static_text("\"O\"               Light sensors\r\n");
                        uart1_send_static_text("\"P,#,#\"           Set motor position left,right\r\n");
                        uart1_send_static_text("\"Q\"               Get motor position left,right\r\n");
                        uart1_send_static_text("\"R\"               Reset e-puck\r\n");
                        uart1_send_static_text("\"S\"               Stop e-puck and turn off leds\r\n");
                        uart1_send_static_text("\"T,#\"             Play sound 1-5 else stop sound\r\n");
                        if (isEpuckVersion1_3()) {
                            uart1_send_static_text("\"t\"               Temperature\r\n");
                        }
                        uart1_send_static_text("\"U\"               Get microphone amplitude\r\n");
                        uart1_send_static_text("\"V\"               Version of SerCom\r\n");
                        uart1_send_static_text("\"W\"               Write I2C (mod,reg,val)\r\n");
                        uart1_send_static_text("\"Y\"               Read I2C val=(mod,reg)\r\n");
                    } else { // when working with the gumstix extension some commands are no more available
                        uart2_send_static_text("\n");
                        if (isEpuckVersion1_3() == 0) {
                            uart2_send_static_text("\"A\"       Accelerometer\r\n");
                        }
                        if (isEpuckVersion1_3()) {
                            uart2_send_static_text("\"b\"       Battery value\r\n");
                        } else {
                            uart2_send_static_text("\"b\"       Battery state (1=ok, 0=low)\r\n");
                        }
                        //uart2_send_static_text("\"B,#\"       Body led 0=off 1=on 2=inverse\r\n");
                        uart2_send_static_text("\"C\"       Selector position\r\n");
                        uart2_send_static_text("\"D,#,#\"   Set motor speed left,right\r\n");
                        uart2_send_static_text("\"E\"       Get motor speed left,right\r\n");
                        //uart2_send_static_text("\"F,#\"       Front led 0=off 1=on 2=inverse\r\n");
#ifdef IR_RECEIVER
                        uart2_send_static_text("\"G\"       IR receiver\r\n");
#endif
                        uart2_send_static_text("\"H\"       Help\r\n");
                        //uart2_send_static_text("\"I\"         Get camera parameter\r\n");
                        //uart2_send_static_text("\"J,#,#,#,#,#,#\" Set camera parameter mode,width,heigth,zoom(1,4 or 8),x1,y1\r\n");
                        uart2_send_static_text("\"K\"       Calibrate proximity sensors\r\n");
                        uart2_send_static_text("\"L,#,#\"   Led number,0=off 1=on 2=inverse\r\n");
#ifdef FLOOR_SENSORS
                        uart2_send_static_text("\"M\"       Floor sensors\r\n");
#endif
                        uart2_send_static_text("\"N\"       Proximity\r\n");
                        uart2_send_static_text("\"O\"       Light sensors\r\n");
                        uart2_send_static_text("\"P,#,#\"   Set motor position left,right\r\n");
                        uart2_send_static_text("\"Q\"       Get motor position left,right\r\n");
                        uart2_send_static_text("\"R\"       Reset e-puck\r\n");
                        uart2_send_static_text("\"S\"       Stop e-puck and turn off leds\r\n");
                        //uart2_send_static_text("\"T,#\"       Play sound 1-5 else stop sound\r\n");
                        uart2_send_static_text("\"U\"       Get microphone amplitude\r\n");
                        uart2_send_static_text("\"V\"       Version of SerCom\r\n");
                        //uart2_send_static_text("\"W\"         Write I2C (mod,reg,val)\r\n");
                        //uart2_send_static_text("\"Y\"         Read I2C val=(mod,reg)\r\n");
                    }
                    break;
                case 'I':
                    if (use_bt) {
                        sprintf(buffer, "i,%d,%d,%d,%d,%d\r\n", cam_mode, cam_width, cam_heigth, cam_zoom, cam_size);
                        uart1_send_text(buffer);
                    }
                    break;
                case 'J'://set camera parameter see also cam library
                    if (use_bt) {
                        cam_x1 = -1;
                        cam_y1 = -1;
                        sscanf(buffer, "J,%d,%d,%d,%d,%d,%d\r", &cam_mode, &cam_width, &cam_heigth, &cam_zoom, &cam_x1, &cam_y1);
                        if (cam_mode == GREY_SCALE_MODE)
                            cam_size = cam_width * cam_heigth;
                        else
                            cam_size = cam_width * cam_heigth * 2;
                        if (cam_size > BUFFER_SIZE) { // if desired settings too demanding set to a reasonable default
                            cam_mode = RGB_565_MODE;
                            cam_width = 40; // DEFAULT_WIDTH;
                            cam_heigth = 40; // DEFAULT_HEIGHT;
                            cam_size = cam_width * cam_heigth * 2;
                        }
                        e_poxxxx_init_cam();
                        if (cam_x1 == -1) { // user did not specify: take default
                            cam_x1 = (ARRAY_WIDTH - cam_width * cam_zoom) / 2;
                        }
                        if (cam_y1 == -1) { // user did not specify: take default
                            cam_y1 = (ARRAY_HEIGHT - cam_heigth * cam_zoom) / 2;
                        }
                        e_poxxxx_config_cam(cam_x1, cam_y1, cam_width*cam_zoom, cam_heigth*cam_zoom, cam_zoom, cam_zoom, cam_mode);
                        e_poxxxx_write_cam_registers();
                        uart1_send_static_text("j\r\n");
                    }
                    break;
                case 'K': // calibrate proximity sensors
                    if (use_bt) {
                        uart1_send_static_text("k, Starting calibration - Remove any object in sensors range\r\n");
                    } else {
                        uart2_send_static_text("k, Starting calibration - Remove any object in sensors range\r\n");
                    }
                    int long t;
                    e_set_led(8, 1);
                    for (t = 0; t < 1000000; ++t);
                    e_led_clear();
                    for (t = 0; t < 10000; ++t);
                    e_calibrate_ir();
                    if (use_bt) {
                        uart1_send_static_text("k, Calibration finished\r\n");
                    } else {
                        uart2_send_static_text("k, Calibration finished\r\n");
                    }
                    break;
                case 'L': // set led
                    sscanf(buffer, "L,%d,%d\r", &LED_nbr, &LED_action);
                    e_set_led(LED_nbr, LED_action);
                    if (use_bt) {
                        uart1_send_static_text("l\r\n");
                    } else {
                        uart2_send_static_text("l\r\n");
                    }
                    break;
                case 'M': // read floor sensors (optional)
#ifdef FLOOR_SENSORS
                    if (use_bt) {
                        e_i2cp_enable();
                        for (j = 0; j < 6; j++) {
                            if (j % 2 == 0) buffer[j] = e_i2cp_read(0xC0, j + 1);
                            else buffer[j] = e_i2cp_read(0xC0, j - 1);
                        }
#ifdef CLIFF_SENSORS
                        for (j = 13; j < 17; j++) {
                            if (j % 2 == 0) buffer[j - 7] = e_i2cp_read(0xC0, j - 1);
                            else buffer[j - 7] = e_i2cp_read(0xC0, j + 1);
                        }
#endif
                        e_i2cp_disable();

#ifdef CLIFF_SENSORS
                        sprintf(buffer, "m,%d,%d,%d,%d,%d\r\n",
                                (unsigned int) (buffer[0] & 0xff) +
                                ((unsigned int) buffer[1] << 8),
                                (unsigned int) (buffer[2] & 0xff) +
                                ((unsigned int) buffer[3] << 8),
                                (unsigned int) (buffer[4] & 0xff) +
                                ((unsigned int) buffer[5] << 8),
                                (unsigned int) (buffer[6] & 0xff) +
                                ((unsigned int) buffer[7] << 8),
                                (unsigned int) (buffer[8] & 0xff) +
                                ((unsigned int) buffer[9] << 8));
#else
                        sprintf(buffer, "m,%d,%d,%d\r\n",
                                (unsigned int) (buffer[0] & 0xff) +
                                ((unsigned int) buffer[1] << 8),
                                (unsigned int) (buffer[2] & 0xff) +
                                ((unsigned int) buffer[3] << 8),
                                (unsigned int) (buffer[4] & 0xff) +
                                ((unsigned int) buffer[5] << 8));
#endif
                        uart1_send_text(buffer);
                    }
#else
                    if (use_bt) {
                        uart1_send_static_text("m,0,0,0\r\n");
                    } else {
                        uart2_send_static_text("m,0,0,0\r\n");
                    }
#endif
                    break;
                case 'N': // read proximity sensors
                    if (use_bt) {
                        sprintf(buffer, "n,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
                                e_get_calibrated_prox(0), e_get_calibrated_prox(1), e_get_calibrated_prox(2), e_get_calibrated_prox(3),
                                e_get_calibrated_prox(4), e_get_calibrated_prox(5), e_get_calibrated_prox(6), e_get_calibrated_prox(7));
                        uart1_send_text(buffer);
                    } else {
                        sprintf(buffer, "n,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
                                e_get_calibrated_prox(0), e_get_calibrated_prox(1), e_get_calibrated_prox(2), e_get_calibrated_prox(3),
                                e_get_calibrated_prox(4), e_get_calibrated_prox(5), e_get_calibrated_prox(6), e_get_calibrated_prox(7),
                                e_get_calibrated_prox(8), e_get_calibrated_prox(9));
                        uart2_send_text(buffer);
                    }
                    break;
                case 'O': // read ambient light sensors
                    if (use_bt) {
                        sprintf(buffer, "o,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
                                e_get_ambient_light(0), e_get_ambient_light(1), e_get_ambient_light(2), e_get_ambient_light(3),
                                e_get_ambient_light(4), e_get_ambient_light(5), e_get_ambient_light(6), e_get_ambient_light(7));
                        uart1_send_text(buffer);
                    } else {
                        sprintf(buffer, "o,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
                                e_get_ambient_light(0), e_get_ambient_light(1), e_get_ambient_light(2), e_get_ambient_light(3),
                                e_get_ambient_light(4), e_get_ambient_light(5), e_get_ambient_light(6), e_get_ambient_light(7),
                                e_get_ambient_light(8), e_get_ambient_light(9));
                        uart2_send_text(buffer);
                    }
                    break;
                case 'P': // set motor position
                    sscanf(buffer, "P,%d,%d\r", &positionl, &positionr);
                    e_set_steps_left(positionl);
                    e_set_steps_right(positionr);
                    if (use_bt) {
                        uart1_send_static_text("p\r\n");
                    } else {
                        uart2_send_static_text("p\r\n");
                    }
                    break;
                case 'Q': // read motor position
                    sprintf(buffer, "q,%d,%d\r\n", e_get_steps_left(), e_get_steps_right());
                    if (use_bt) {
                        uart1_send_text(buffer);
                    } else {
                        uart2_send_text(buffer);
                    }
                    break;
                case 'R': // reset
                    if (use_bt) {
                        uart1_send_static_text("r\r\n");
                    } else {
                        uart2_send_static_text("r\r\n");
                    }
                    RESET();
                    break;
                case 'S': // stop
                    e_set_speed_left(0);
                    e_set_speed_right(0);
                    e_set_led(8, 0);
                    if (use_bt) {
                        uart1_send_static_text("s\r\n");
                    } else {
                        uart2_send_static_text("s\r\n");
                    }
                    break;
                case 'T': // stop
                    if (use_bt) {
                        sscanf(buffer, "T,%d", &sound);
                        if (first == 0) {
                            e_init_sound();
                            first = 1;
                        }
                        switch (sound) {
                            case 1: e_play_sound(0, 2112);
                                break;
                            case 2: e_play_sound(2116, 1760);
                                break;
                            case 3: e_play_sound(3878, 3412);
                                break;
                            case 4: e_play_sound(7294, 3732);
                                break;
                            case 5: e_play_sound(11028, 8016);
                                break;
                            default:
                                e_close_sound();
                                first = 0;
                                break;
                        }
                        uart1_send_static_text("t\r\n");
                    } else {
                        uart2_send_static_text("t\r\n");
                    }
                    break;

                case 't': // temperature
                    if (use_bt && isEpuckVersion1_3()) {
                        sprintf(buffer, "t,%d\r\n", getTemperature());
                    } else {
                        sprintf(buffer, "t,0\r\n");
                    }
                    if (use_bt) {
                        uart1_send_text(buffer);
                    } else {
                        uart2_send_text(buffer);
                    }
                    break;

                case 'U':
                    sprintf(buffer, "u,%d,%d,%d\r\n", e_get_micro_volume(0), e_get_micro_volume(1), e_get_micro_volume(2));
                    if (use_bt) {
                        uart1_send_text(buffer);
                    } else {
                        uart2_send_text(buffer);
                    }
                    break;
                case 'V': // get version information
                    if (use_bt) {
                        uart1_send_static_text("v,Version 1.2.2 August 2008 GCtronic\r\n");
                    } else {
                        uart2_send_static_text("v,Version 1.2.2 August 2008 GCtronic\r\n");
                    }
                    sprintf(buffer, "HW version: %X\r\n", HWversion);
                    if (use_bt) {
                        uart1_send_text(buffer);
                    } else {
                        uart2_send_text(buffer);
                    }
                    break;
                case 'W': // write I2C message
                    if (use_bt) {
                        sscanf(buffer, "W,%d,%d,%d\r", &mod, &reg, &val);
                        e_i2cp_enable();
                        e_i2cp_write((char) mod, (char) reg, (char) val); // write I2C
                        e_i2cp_disable();
                        uart1_send_static_text("w\r\n");
                    }
                    break;
                case 'Y': // read I2C message
                    if (use_bt) {
                        sscanf(buffer, "Y,%d,%d\r", &mod, &reg);

                        sprintf(buffer, "y,%d,%d\r\n", mod, reg);
                        uart1_send_text(buffer);




                        e_i2cp_enable();
                        val = e_i2cp_read((char) mod, (char) reg); // read I2C
                        e_i2cp_disable();
                        sprintf(buffer, "y,%d\r\n", val);
                        uart1_send_text(buffer);
                    }
                    break;
                case 'Z': // scann I2C addresses
                    if (use_bt) {
                        for (j = 2; j < 255; j = j + 2) {
                            e_i2cp_enable();
                            val = e_i2cp_read((char) j, 0); // read I2C
                            e_i2cp_disable();
                            if (val >= 0) {
                                sprintf(buffer, "%d: %d\r\n", j, val);
                                uart1_send_text(buffer);
                            }
                        }
                    }
                    break;
                default:
                    if (use_bt) {
                        uart1_send_static_text("z,Command not found\r\n");
                    } else {
                        uart2_send_static_text("z,Command not found\r\n");
                    }
                    break;
            }
        }
    }
}
