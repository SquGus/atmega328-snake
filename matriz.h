//LibrerÌa para controlar la matriz de leds

 #include <delay.h>
 #define DIN PORTC.0
 #define LOAD PORTC.1
 #define CLK PORTC.2
    
flash char Letras[133][5] = {{0,0,0,0,0},    //espacio
    {0,0,242,0,0        },    // !
    {0,224,0,224,0      },    // "
    {40,254,40,254,40   },    // #
    {36,84,254,84,72    },    // $
    {196,200,16,38,70   },    // %
    {108,146,170,132,10 },    // &
    {0,160,192,0,0      },    // Ô
    {0,56,68,130,0      },    // (
    {0,130,68,56,0      },    // )
    {40,16,124,16,40    },    // *
    {16,16,124,16,16    },    // +
    {0,5,6,0,0          },    // ,
    {16,16,16,16,16     },    // -
    {0,6,6,0,0          },    // .
    {4,8,16,32,64       },    // /
    {124,138,146,162,124},    // 0
    {0,66,254,2,0       },    // 1
    {66,134,138,146,98  },    // 2
    {132,130,162,226,156},    // 3
    {24,40,72,254,8     },    // 4
    {228,162,162,162,156},    // 5
    {60,82,146,146,12   },    // 6
    {128,142,144,160,192},    // 7
    {108,146,146,146,108},    // 8
    {96,146,146,148,120 },    // 9
    {0,54,54,0,0        },    // :
    {0,53,54,0,0        },    // ;
    {16,40,68,130,0     },    // <
    {40,40,40,40,40     },    // =
    {0,130,68,40,16     },    // >
    {64,128,138,144,96  },    // ?
    {76,146,158,130,124 },    // @
    {126,144,144,144,126},    // A
    {254,146,146,146,108},    // B 
    {124,130,130,130,68 },    // C
    {254,130,130,130,124},    // D
    {254,146,146,146,146},    // E
    {254,144,144,144,144},    // F
    {124,130,146,146,92 },    // G
    {254,16,16,16,254   },    // H
    {0,130,254,130,0    },    // I
    {132,130,130,130,252},    // J
    {254,16,40,68,130   },    // K
    {254,2,2,2,2        },    // L
    {254,64,32,64,254   },    // M
    {254,64,32,16,254   },    // N
    {124,130,130,130,124},    // O
    {254,144,144,144,96 },    // P
    {124,130,134,130,125},    // Q
    {254,144,152,148,98 },    // R
    {100,146,146,146,76 },    // S
    {128,128,254,128,128},    // T
    {252,2,2,2,252      },    // U
    {248,4,2,4,248      },    // V
    {254,4,8,4,254      },    // W
    {198,40,16,40,198   },    // X
    {192,32,30,32,192   },    // Y
    {134,138,146,162,194},    // Z
    {0,254,130,130,0    },    // [
    {64,32,16,8,4       },    // \
    {0,130,130,254,0    },    // ]
    {32,64,128,64,32    },    // ^
    {2,2,2,2,2          },    // _
    {0,128,64,32,0      },    // `
    {4,42,42,42,30      },    // a
    {254,18,34,34,28    },    // b
    {28,34,34,34,34     },    // c
    {28,34,34,18,254    },    // d
    {28,42,42,42,24     },    // e
    {16,126,144,128,64  },    // f
    {24,37,37,37,62     },    // g
    {254,16,32,32,30    },    // h
    {0,34,190,2,0       },    // i
    {4,2,34,188,0       },    // j
    {254,8,20,34,0      },    // k
    {0,130,254,2,0      },    // l
    {62,32,24,32,30     },    // m
    {62,16,32,32,30     },    // n
    {28,34,34,34,28     },    //  
    {63,36,36,36,24     },    // p
    {24,36,36,36,63     },    // q
    {62,16,32,32,16     },    // r
    {18,42,42,42,4      },    // s
    {32,254,34,2,4      },    // t
    {60,2,2,4,62        },    // u
    {56,4,2,4,56        },    // v
    {60,2,12,2,60       },    // w
    {34,20,8,20,34      },    // x
    {48,10,10,10,60     },    // y
    {34,38,42,50,34     },    // z
    {0,16,108,130,0     },    // {
    {0,0,254,0,0        },    // |
    {0,130,108,16,0     },    // }
    {16,32,16,8,16      },    // ~
    {0,0,0,0,0          },    //; 
    {4,170,42,170,30    },    //Ñ 
    {28,162,34,162,28   },    //  
    {28,66,2,66,28      },    // Å
    {62,208,80,208,62   },    // é
    {60,194,66,194,60   },	  // ô
    {60,130,2,130,60    },	  // ö
    {127,144,146,146,108}};	  // ·

void MandaMax7219(unsigned int dato)
{
    unsigned char i;
    CLK=0;    
    LOAD=0;
    //ciclo para mandar bit por bit (serial)
    for(i=0;i<16;i++)
    {
        if((dato&0x8000)==0)
            DIN=0;
        else
            DIN=1;
        CLK=1;
        CLK=0;
        dato=dato<<1; //esto es lo mismo a dividirlo entre 2, para recorrer datos    
    }
    LOAD=1;
    LOAD=0;
}




void ConfiguraMax(void)
{
    DDRC|=0x07;
    MandaMax7219(0x0900);
    MandaMax7219(0x0A0B);
    MandaMax7219(0x0B07);
    MandaMax7219(0x0C01);
    MandaMax7219(0x0F00);
}
//DespliegaLetra(unsigned char c).- procedimiento que recibe como par·metro
//la letra que se quiere desplegar en la matriz. Esta deber· accesar la tabla
//Letras para la generaciÛn de las letras. Existe un desfasamiento de 32
//entre el primer elemento de la tabla y su correspondiente ASCII (el primer elemento
//de la tabla es el del espacio cuyo ASCII es 32).
void DespliegaLetra(unsigned char c)
{
    unsigned int i;
    c = c-32;  
    MandaMax7219(0x0100);
    MandaMax7219(0x0200);
    MandaMax7219(0x0800); 
    for(i=3;i<8;i++)
        MandaMax7219((i<<8)|Letras[c][7-i]);
}

//DespliegaMensaje(unsigned char Mensaje[ ], unsigned int tiempo).
//Procedimiento que recibe un string con el mensaje a mandar a la matriz y
//un entero con el n˙mero de milisegundos que tardar· en mandar cada letra.
//C por default cuando t˙ defines un string le asigna un car·cter nulo al final
//del string para identificar cu·ndo termina este arreglo de caracteres.
void DespliegaMensaje(flash char Cadena[],unsigned int tiempo)
{
    unsigned char i=0;
    while(Cadena[i]!=0)
    {
        DespliegaLetra(Cadena[i++]);
        delay_ms(tiempo);        
    }
}
