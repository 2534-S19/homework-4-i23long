#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

int main(void)
{

    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";

    // TODO: Declare the variables that main uses to interact with your state machine.

    bool finished = false;
    int i=0;
    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_Config uartConfig = {
         EUSCI_A_UART_CLOCKSOURCE_SMCLK,               // SMCLK Clock Source = 3MHz
         19,                                           // UCBR   = 312
         8,                                            // UCBRF  = 8
         0x55,                                         // UCBRS  = 0x55
         EUSCI_A_UART_NO_PARITY,                       // No Parity
         EUSCI_A_UART_LSB_FIRST,                       // LSB First
         EUSCI_A_UART_ONE_STOP_BIT,                    // One stop bit
         EUSCI_A_UART_MODE,                            // UART mode
         EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION // Oversampling
    };
    InitUART(EUSCI_A0_BASE, &uartConfig, GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3);

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.


    // TODO: Initialize EUSCI_A0


    // TODO: Enable EUSCI_A0


    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        if (UARTHasChar(EUSCI_A0_BASE))
        {
            rChar = UARTGetChar(EUSCI_A0_BASE);

            if (UARTCanSend(EUSCI_A0_BASE))
                        UARTPutChar(EUSCI_A0_BASE, rChar);
            finished=charFSM( rChar);
        }


        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if(rChar=='\r'&&finished == true)
        {
            if (UARTCanSend(EUSCI_A0_BASE))
            {
                while(1)
                {
                    UARTPutChar(EUSCI_A0_BASE, response[i]);
                    i++;
                    if(i==48)
                    {
                        i=0;
                        break;
                    }
                }
                finished=false;
            }
        }


        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.



    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    bool finished = false;
    if(rChar=='\r')
        finished=true;

    return finished;
}
void UpdateUART(uint32_t moduleInstance,
                const eUSCI_UART_Config *uartConfig_p)
{
    UART_initModule(moduleInstance, uartConfig_p);
    UART_enableModule(moduleInstance);
}
void InitUART(uint32_t moduleInstance,
              const eUSCI_UART_Config *uartConfig_p,
              uint_fast8_t selectedPort, uint_fast16_t selectedPins)
{
        UpdateUART(moduleInstance, uartConfig_p);
        GPIO_setAsPeripheralModuleFunctionInputPin(selectedPort,
                selectedPins, GPIO_PRIMARY_MODULE_FUNCTION);
}


// check the state if it's ready to receive data
bool UARTHasChar(uint32_t moduleInstance)
{
    return UART_getInterruptStatus (moduleInstance, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG;
}
// return the char
uint8_t UARTGetChar(uint32_t moduleInstance)
{
    return UART_receiveData(moduleInstance);
}
//check the state if it's ready to send data
bool UARTCanSend(uint32_t moduleInstance)
{
    return UART_getInterruptStatus (EUSCI_A0_BASE, EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG;
}
// transmit data
void UARTPutChar(uint32_t moduleInstance, uint8_t tChar)
{
    UART_transmitData(moduleInstance, tChar);
}
