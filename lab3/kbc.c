#include "kbc.h"
#include "i8042.h"

int (verify_errors)(uint8_t value) {                                  // Função auxiliar que vê se valor tem
    return ((value >> 6) > 0);                                      // erros de paridade ou timout
}

int (read_kbc_status)(uint8_t* status) {                            // Lê o valor em STAT_REG para ver o estado
    return util_sys_inb(STAT_REG, status);                         // e mete em status
}

int (read_kbc_output)(uint8_t *output) {
    uint8_t status;
    uint8_t attempts = 10;

    while (attempts) {
        if (read_kbc_status(&status) != 0) return 1;                // Lê o valor de STAT_REG e mete em status

        if ((status & OBF) && (!verify_errors(status))) {           // Verifica se OBF está cheio + procura por erros
            if (util_sys_inb(BUF, output) != 0) return 1;          // Mete o conteúdo no OUT_BUF em output
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        attempts--;
    }
    return 1;                                                       // Ultrapassou attempts
}

int (write_kbc_command)(uint8_t command_byte) {
    uint8_t status;
    uint8_t attempts = 10;
    
    while (attempts) {
        if (read_kbc_status(&status) != 0) return 1;                // Lê o valor de STAT_REG e mete em status
        // Queremos ver se IN_BUF está vazio para sabermos se podemos escrever nele ou não um comando
        if (!(status & IBF) && (!verify_errors(status))) {          // Procura por erros e vê se IN_BUF está cheio
            if (sys_outb(STAT_REG, WRITE_COMMAND) != 0) return 1;   // Escreve o comando de escrita no STAT_REG
            if (sys_outb(BUF, command_byte) != 0) return 1;         // Escreve o command byte em IN_BUF para pôr o comando
        }
        tickdelay(micros_to_ticks(20000));
        attempts--;
    }
    return 1;                                                       // Ultrapassou attempts
}
