/*
 *  cpu.c
 *  Contains APEX cpu pipeline implementation
 *
 *  Author :
 *  Gaurav Kothari (gkothar1@binghamton.edu)
 *  State University of New York, Binghamton
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"

/* Set this flag to 1 to enable debug messages */
#define ENABLE_DEBUG_MESSAGES 1
//CPU_Stage* stageE;
int stallFlag = 0;

/*
 * This function creates and initializes APEX cpu.
 *
 * Note : You are free to edit this function according to your
 *                 implementation
 */
APEX_CPU*
APEX_cpu_init(const char* filename)
{
    if (!filename) {
        return NULL;
    }
    
    APEX_CPU* cpu = malloc(sizeof(*cpu));
    if (!cpu) {
        return NULL;
    }
    
    /* Initialize PC, Registers and all pipeline stages */
    cpu->pc = 4000;
    memset(cpu->regs, 0, sizeof(int) * 16);
    memset(cpu->regs_valid, 1, sizeof(int) * 16);
    memset(cpu->stage, 0, sizeof(CPU_Stage) * NUM_STAGES);
    memset(cpu->data_memory, 0, sizeof(int) * 4000);
    
    /* Parse input file and create code memory */
    cpu->code_memory = create_code_memory(filename, &cpu->code_memory_size);
    
    if (!cpu->code_memory) {
        free(cpu);
        return NULL;
    }
    
    if (ENABLE_DEBUG_MESSAGES) {
        fprintf(stderr,
                "APEX_CPU : Initialized APEX CPU, loaded %d instructions\n",
                cpu->code_memory_size);
        fprintf(stderr, "APEX_CPU : Printing Code Memory\n");
        printf("%-9s %-9s %-9s %-9s %-9s\n", "opcode", "rd", "rs1", "rs2", "imm");
        
        for (int i = 0; i < cpu->code_memory_size; ++i) {
            printf("%-9s %-9d %-9d %-9d %-9d\n",
                   cpu->code_memory[i].opcode,
                   cpu->code_memory[i].rd,
                   cpu->code_memory[i].rs1,
                   cpu->code_memory[i].rs2,
                   cpu->code_memory[i].imm);
        }
    }
    
    /* Make all stages busy except Fetch stage, initally to start the pipeline */
    for (int i = 1; i < NUM_STAGES; ++i) {
        cpu->stage[i].busy = 1;
    }
    
    return cpu;
}

/*
 * This function de-allocates APEX cpu.
 *
 * Note : You are free to edit this function according to your
 *                 implementation
 */
void
APEX_cpu_stop(APEX_CPU* cpu)
{
    free(cpu->code_memory);
    free(cpu);
}

/* Converts the PC(4000 series) into
 * array index for code memory
 *
 * Note : You are not supposed to edit this function
 *
 */
int
get_code_index(int pc)
{
    return (pc - 4000) / 4;
}

static void
print_instruction(CPU_Stage* stage)
{
    if (strcmp(stage->opcode, "STORE") == 0) {
        printf(
               "%s,R%d,R%d,#%d ", stage->opcode, stage->rs1, stage->rs2, stage->imm);
    }
    
    if (strcmp(stage->opcode, "LOAD") == 0) {
        printf("%s,R%d,R%d,R%d ", stage->opcode, stage->rd, stage->rs1, stage->imm);
    }
    
    if (strcmp(stage->opcode, "MOVC") == 0) {
        printf("%s,R%d,#%d ", stage->opcode, stage->rd, stage->imm);
    }
    
    if ((strcmp(stage->opcode, "ADD") == 0) || (strcmp(stage->opcode, "AND") == 0) || (strcmp(stage->opcode, "OR") == 0) || (strcmp(stage->opcode, "XOR") == 0)) {
        printf("%s,R%d,R%d,R%d ", stage->opcode, stage->rd, stage->rs1, stage->rs2);
    }
    
    if (strcmp(stage->opcode, "SUB") == 0) {
        printf("%s,R%d,R%d,R%d ", stage->opcode, stage->rd, stage->rs1, stage->rs2);
    }
    
    if (strcmp(stage->opcode, "MUL") == 0) {
        printf("%s,R%d,R%d,R%d ", stage->opcode, stage->rd, stage->rs1, stage->rs2);
    }
}

/* Debug function which dumps the cpu stage
 * content
 *
 * Note : You are not supposed to edit this function
 *
 */
static void
print_stage_content(char* name, CPU_Stage* stage)
{
    printf("%-15s: pc(%d) ", name, stage->pc);
    print_instruction(stage);
    printf("\n");
}

static void
print_empty_stage_content(char* name)
{
    printf("%-15s: pc(0) ", name);
    printf("\n");
}

static void
print_empty_stage_fetch(char* name, int pc)
{
    printf("%-15s: pc(%d) ", name, pc);
    printf("\n");
}

/*
 *  Fetch Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
fetch(APEX_CPU* cpu)
{
    CPU_Stage* stage = &cpu->stage[F];
    //    if(stallFlag) {
    //        stage->stalled = 1;
    //        if (ENABLE_DEBUG_MESSAGES) {
    //            print_stage_content("Fetch", stage);
    //        }
    //    } else {
    //        stage->stalled = 0;
    //    }
    //
//    if(cpu->stage[DRF].stalled == 1) {
//        stage->stalled = 1;
//    } else {
//        stage->stalled = 0;
//    }
    if (!stage->busy && !stage->stalled) {
        /* Store current PC in fetch latch */
        stage->pc = cpu->pc;
        
        /* Index into code memory using this pc and copy all instruction fields into
         * fetch latch
         */
        APEX_Instruction* current_ins = &cpu->code_memory[get_code_index(cpu->pc)];
        strcpy(stage->opcode, current_ins->opcode);
        stage->rd = current_ins->rd;
        stage->rs1 = current_ins->rs1;
        stage->rs2 = current_ins->rs2;
        stage->imm = current_ins->imm;
        //stage->rd = current_ins->rd;
        
        /* To check if destination register is valid or not 0:valid, 1: not valid*/
//        if (cpu->regs_valid[stage->rd] == 1) {
//            cpu->regs_valid[stage->rd]++;
//        } else if (stage->rd == stage->rs1 || stage->rd==stage->rs2) {
//            cpu->regs_valid[stage->rd] = cpu->regs_valid[stage->rd];
//        }
//
//        else {
//            cpu->regs_valid[stage->rd] = 1;
//        }
        
        if(cpu->stage[DRF].stalled == 1) {
//            printf("\nInside fetch stall cond: %s", stage->opcode);
            //stage->stalled = 1;
            if (ENABLE_DEBUG_MESSAGES) {
                print_stage_content("Fetch", stage);
            }
            return 0;
//        } else {
//            stage->stalled = 0;
        }
        
        /* Update PC for next instruction */
        cpu->pc += 4;
        
        /* Copy data from fetch latch to decode latch*/
        cpu->stage[DRF] = cpu->stage[F];
        
        if (ENABLE_DEBUG_MESSAGES) {
            print_stage_content("Fetch", stage);
        }
//    } else if (stage->stalled == 1) {
//        print_stage_content("Fetch", stage);
    } else {
        print_empty_stage_fetch("Fetch", cpu->pc);
    }
    return 0;
}

/*
 *  Decode Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
decode(APEX_CPU* cpu)
{
    CPU_Stage* stage = &cpu->stage[DRF];
    //    if(stallFlag) {
    //        stage->stalled = 1;
    //        if (ENABLE_DEBUG_MESSAGES) {
    //            print_stage_content("Decode/RF", stage);
    //        }
    //    } else {
    //        stage->stalled = 0;
    //    }
    int validFlag = 0;
    if (cpu->stage[EX].busy && (strcmp(cpu->stage[EX].opcode, "MUL") == 0)) {
        stage->stalled = 1;
    } else if (!cpu->stage[EX].busy && (strcmp(cpu->stage[EX].opcode, "MUL") == 0)) {
//        printf("!cpu->stage[EX].busy && (strcmp(cpu->stage[EX].opcode, MUL) == 0");
        stage->stalled = 0;
    }
    if (stallFlag) {
        stage->stalled = 0;
//        printf("\nupdated stage->stalled = 0");
    }
    if (!stage->busy && !stage->stalled) {
        
        /* Read data from register file for store */
        if (strcmp(stage->opcode, "STORE") == 0) {
            if (cpu->regs_valid[stage->rs2]==0 && cpu->regs_valid[stage->rs1]==0) {
                stage->rs2_value = cpu->regs[stage->rs2];
                stage->rs1_value = cpu->regs[stage->rs1];
            } else {
                validFlag = 1;
            }
        }
        
        /* Read data from register file for load */
        if (strcmp(stage->opcode, "LOAD") == 0) {
            if (cpu->regs_valid[stage->rs1] == 0) {
                stage->rs1_value = cpu->regs[stage->rs1];
            } else {
                validFlag = 1;
            }
        }
        
        /* No Register file read needed for MOVC */
        if (strcmp(stage->opcode, "MOVC") == 0) {
        }
        
        /* ADD, AND, OR, XOR, MUL, SUB */
        if ((strcmp(stage->opcode, "ADD") == 0) || (strcmp(stage->opcode, "AND") == 0) || (strcmp(stage->opcode, "OR") == 0) || (strcmp(stage->opcode, "XOR") == 0) || (strcmp(stage->opcode, "SUB") == 0) || (strcmp(stage->opcode, "MUL") == 0)) {
            if (cpu->regs_valid[stage->rs1]==0 && cpu->regs_valid[stage->rs2]==0) {
                stage->rs2_value = cpu->regs[stage->rs2];
                stage->rs1_value = cpu->regs[stage->rs1];
            } else {
                validFlag = 1;
            }
        }
        
        if(validFlag) {
            stage->stalled = 1;
            stallFlag = 1;
//            printf("\nupdated stallFlag = 1");
//            if (ENABLE_DEBUG_MESSAGES) {
//                print_stage_content("Decode/RF", stage);
//            }
//            return 0;
        } else {
            stallFlag = 0;
//            printf("\nupdated stallFlag = 0");
        }
        
        /* Copy data from decode latch to execute latch*/
        //        if(!stage->stalled) {
        cpu->stage[EX] = cpu->stage[DRF];
        //        }
        
        
        if (cpu->regs_valid[stage->rd] == 1) {
            cpu->regs_valid[stage->rd]++;
//        } else if (stage->rd == stage->rs1 || stage->rd==stage->rs2) {
//            cpu->regs_valid[stage->rd] = cpu->regs_valid[stage->rd];
        } else {
            cpu->regs_valid[stage->rd] = 1;
        }
        
        
        if (ENABLE_DEBUG_MESSAGES) {
            print_stage_content("Decode/RF", stage);
        }
    } else if (stage->stalled == 1) {
        print_stage_content("Decode/RF", stage);
    } else {
        print_empty_stage_content("Decode/RF");
    }
    return 0;
}

/*
 *  Execute Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
execute(APEX_CPU* cpu)
{
    CPU_Stage* stage = &cpu->stage[EX];
    //    if(stage->stalled) {
    ////        stage->stalled = 0;
    //        if (ENABLE_DEBUG_MESSAGES) {
    //            print_stage_content("Execute", stage);
    //        }
    //        cpu->stage[EX] = cpu->stage[EX];
    //        cpu->stage[MEM] = cpu->stage[EX];
    //        stage->stalled = 0;
    //        stallFlag = 1;
    //        return 0;
    //    }
    if ((strcmp(stage->opcode, "MUL") == 0) && stage->busy == 0 && stage->stalled == 0) {
//        printf("\nhello\n");
        stage->busy = 1;
        cpu->stage[MEM] = cpu->stage[EX];
        if (ENABLE_DEBUG_MESSAGES) {
            print_stage_content("Execute", stage);
        }
        return 0;
    } else if ((strcmp(stage->opcode, "MUL") == 0) && stage->busy == 1) {
//        printf("\nworld\n");
        stage->busy = 0;
    }
    
    
    if (!stage->busy && !stage->stalled) {
        //        stallFlag = 0;
        /* Store */
        if (strcmp(stage->opcode, "STORE") == 0) {
            stage->mem_address = stage->rs2_value + stage->imm;
        }
        
        /* Store */
        if (strcmp(stage->opcode, "LOAD") == 0) {
            stage->mem_address = stage->rs1_value + stage->imm;
        }
        
        /* MOVC */
        if (strcmp(stage->opcode, "MOVC") == 0) {
            stage->buffer = stage->imm + 0;
        }
        
        /* ADD*/
        if (strcmp(stage->opcode, "ADD") == 0) {
            stage->buffer = stage->rs1_value + stage->rs2_value;
        }
        
        /* SUB*/
        if (strcmp(stage->opcode, "SUB") == 0) {
            stage->buffer = stage->rs1_value - stage->rs2_value;
        }
        
        /* MUL */
        if (strcmp(stage->opcode, "MUL") == 0) {
            stage->buffer = stage->rs1_value * stage->rs2_value;
            //            stage->busy = 1;
        }
        
        /* AND */
        if (strcmp(stage->opcode, "AND") == 0) {
            stage->buffer = stage->rs1_value & stage->rs2_value;
        }
        
        /* OR */
        if (strcmp(stage->opcode, "OR") == 0) {
            stage->buffer = stage->rs1_value | stage->rs2_value;
        }
        
        /* XOR */
        if (strcmp(stage->opcode, "XOR") == 0) {
            stage->buffer = stage->rs1_value ^ stage->rs2_value;
        }
        
        /* Copy data from Execute latch to Memory latch*/
        cpu->stage[MEM] = cpu->stage[EX];
        
        if (ENABLE_DEBUG_MESSAGES) {
            print_stage_content("Execute", stage);
        }
    } else {
        cpu->stage[MEM] = cpu->stage[EX]; //for dependancy
        
        print_empty_stage_content("Execute");
    }
    //  else if(stage->busy == 0) {
    //    stage->busy = 1;
    //    cpu->stage[MEM] = cpu->stage[EX];
    //  }
    
    return 0;
}

/*
 *  Memory Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
memory(APEX_CPU* cpu)
{
    CPU_Stage* stage = &cpu->stage[MEM];
    //    if(stage->stalled) {
    //        print_empty_stage_content("Memory");
    //        strcpy(cpu->stage[WB].opcode, "Stall");
    //        return 0;
    //    }
    if (!stage->busy && !stage->stalled) {
        
        /* Store */
        if (strcmp(stage->opcode, "STORE") == 0) {
            cpu->data_memory[stage->mem_address] = stage->rs1_value;
        }
        
        /* Load */
        if (strcmp(stage->opcode, "LOAD") == 0) {
            stage->buffer = cpu->data_memory[stage->mem_address];
        }
        
        /* MOVC */
        if (strcmp(stage->opcode, "MOVC") == 0) {
        }
        
        /* ADD: Does't do anything*/
        if (strcmp(stage->opcode, "ADD") == 0) {
        }
        
        /* SUB: Does't do anything*/
        if (strcmp(stage->opcode, "SUB") == 0) {
        }
        
        /* MUL: Does't do anything*/
        if (strcmp(stage->opcode, "MUL") == 0) {
        }
        
        /* Copy data from decode latch to execute latch*/
        cpu->stage[WB] = cpu->stage[MEM];
        
        if (ENABLE_DEBUG_MESSAGES) {
            print_stage_content("Memory", stage);\
        }
    } else {
        cpu->stage[WB] = cpu->stage[MEM];
        print_empty_stage_content("Memory");
    }
    return 0;
}

/*
 *  Writeback Stage of APEX Pipeline
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
writeback(APEX_CPU* cpu)
{
    CPU_Stage* stage = &cpu->stage[WB];
    //    printf("\nstalling: %d: %d: %d\n",cpu->stage[MEM].stalled, stage->stalled, stallFlag);
    //    if(strcmp(cpu->stage[WB].opcode, "Stall") == 0) {
    //        print_empty_stage_content("Writeback");
    //        return 0;
    //    }
//    printf("\nwriteback1 %d: %d\n",stage->busy, stage->stalled);
    if (!stage->busy && !stage->stalled) {
        //        printf("\nwriteback2\n");
        
        /* Update register file */
        if (strcmp(stage->opcode, "MOVC") == 0) {
            cpu->regs[stage->rd] = stage->buffer;
            //        printf("\nin write: %d: %d",cpu->regs[stage->rd],stage->buffer);
        }
        
        /* Update register file */
        if (strcmp(stage->opcode, "LOAD") == 0) {
            cpu->regs[stage->rd] = stage->buffer;
        }
        
        /* ADD */
        if (strcmp(stage->opcode, "ADD") == 0) {
            cpu->regs[stage->rd] = stage->buffer;
        }
        
        /* SUB */
        if (strcmp(stage->opcode, "SUB") == 0) {
            cpu->regs[stage->rd] = stage->buffer;
        }
        
        /* MUL */
        if (strcmp(stage->opcode, "MUL") == 0) {
            cpu->regs[stage->rd] = stage->buffer;
        }
        
        if (cpu->regs_valid[stage->rd] == 1) {
            cpu->regs_valid[stage->rd] = 0;
//        } else if (stage->rd == stage->rs1 || stage->rd==stage->rs2) {
//            cpu->regs_valid[stage->rd] = cpu->regs_valid[stage->rd];
        } else {
            cpu->regs_valid[stage->rd]--;
        }
//        printf("\n updated cpu->regs_valid[%d]: %d", stage->rd, cpu->regs_valid[stage->rd]);
        cpu->ins_completed++;
//        printf("\nins_complete: %d\n", cpu->ins_completed);
        
        if (ENABLE_DEBUG_MESSAGES) {
            print_stage_content("Writeback", stage);\
        }
    } else {
        print_empty_stage_content("Writeback");
    }
    return 0;
}

/*
 *  APEX CPU simulation loop
 *
 *  Note : You are free to edit this function according to your
 *                  implementation
 */
int
APEX_cpu_run(APEX_CPU* cpu)
{
    
    //    memset(stageE, 0, sizeof(CPU_Stage) * 1);
    //    strcpy(stageE->opcode, current_ins->opcode);
    //    stageE->rd = "";
    //    stageE->rs1 = current_ins->rs1;
    //    stageE->rs2 = current_ins->rs2;
    //    stageE->imm = current_ins->imm;
    //    stageE->rd = current_ins->rd;
    //    stageE->pc =
    while (1) {
        
        /* All the instructions committed, so exit */
        if (cpu->ins_completed == cpu->code_memory_size) {
            printf("(apex) >> Simulation Complete\n");
            break;
        }
        
        if (ENABLE_DEBUG_MESSAGES) {
            printf("--------------------------------\n");
            printf("Clock Cycle #: %d\n", cpu->clock);
            printf("--------------------------------\n");
        }
        //        printf("F: %d\n",DRF);
        writeback(cpu);
        memory(cpu);
        execute(cpu);
        decode(cpu);
        fetch(cpu);
        
        //      fetch(cpu);
        //      decode(cpu);
        //      execute(cpu);
        //      memory(cpu);
        //      writeback(cpu);
        cpu->clock++;
    }
    
    return 0;
}
