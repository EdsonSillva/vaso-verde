
/*
    Projeto Vaso Verde
    Descrição: Este projeto tem como objetivo cuidar da irrigação do solo de um vaso com planta.
               Serão utilizados um arduino nano, sensor de umidade de solo, um sensor com bóia e 
               uma mini bomba d'agua.
    
    Autor: Edson Silva
    Data: 03/2022

*/

/*
    Define para direcionar a compilação do sketch
    __PROJETO_EM_BRANCO__ true  : Compila um sketch em branco
    __PROJETO_EM_BRANCO__ false : Complia o projeto desenvolvido

*/
#define __PROJETO_EM_BRANCO__                       false
#define __MONITORAR_SERIAL__                        true
#define __ESPERA_ENTRE_LEITURA__                    60000       // 1 minuto
#define __CAPACIDADE_MAX_RECIPIENTE                   100       // 100 = 1 litro

#if(__PROJETO_EM_BRANCO__ == true)

void setup(){ }
void loop() { }

#else

#include "libraries\SensoresLib.h"

#define pinoValorSensorVaso                         A0
#define pinoLigarSensorVaso                         10
#define pinoValorBoia                               03      // Pino com interrupção
#define pinoLigarBombaDagua                         12
#define pinoLigarBoia                               13
#define pinoBuzzer                                  05

#define EEPROM_Address_CapacidadeRecipiente          0
#define EEPROM_Address_JaUsado                       1

SensorVaso                                          Vaso;
SensorBoia                                          Boia;
MiniBombaDagua                                      BombaDagua;
Recipiente                                          RecipienteDagua;

int ValorSensorVaso =                               0x0;
bool ValorSensorBoia =                              false;


/**
 * Definição das funções utilizadas no projeto
 */

void setup();
void loop();
void AlertaSonoro();
//sRecipiente getDadosRecipienteEEPROM();


void setup()
{
    // Inicializa os Sensores
    // ***********************
    Vaso.Iniciar(pinoValorSensorVaso, eComportamento::LigarNaLeitura, pinoLigarSensorVaso);
    Boia.Iniciar(pinoValorBoia,       eComportamento::LigarNaLeitura, pinoLigarBoia);
    BombaDagua.Iniciar(pinoLigarBombaDagua, &Boia);

    RecipienteDagua.Iniciar(__CAPACIDADE_MAX_RECIPIENTE, &Boia, &BombaDagua);

    //Iniclializa pino Buzzer
    pinMode(pinoBuzzer, OUTPUT);

    //Colocar a leitura da EEPROM para recuperar o valor da capacidade do recipiente e seu uso


#if(__MONITORAR_SERIAL__ == true) 
    Serial.begin(9600);
    while(!Serial);
    Serial.print("Monitor serial habilitado - Projeto Vaso Verde\n");
#endif
    

}

void loop()
{

#if(__MONITORAR_SERIAL__ == true) 
    Serial.println("Iniciando monitoramento...\n");
#endif

    ValorSensorVaso = Vaso.getValorSensor();

// #if(__MONITORAR_SERIAL__ == true) 
//     Serial.println("Voltando da leitura do sensor");
//     Serial.print("ValorSensorVaso=");
//     Serial.println(ValorSensorVaso);
// #endif

#if(__MONITORAR_SERIAL__ == true) 
    Serial.print("Estado do solo do vaso: ");
    Serial.print(Vaso.getEstadoDoSoloDescricao());
    Serial.print(" | ");
    Serial.println(ValorSensorVaso);
#endif

    if(Vaso.getEstadoDoSolo() == eUmidadeVaso::Seco)
    {
        // TODO: Verificar se tem água no recipiente
        // TODO: Acionar a bomba de água

#if(__MONITORAR_SERIAL__ == true) 
            Serial.println("\tNecessário irrigar o solo");
            Serial.print("\t\tVerificando se o recipiente tem agua ... ");
#endif
        Boia.getValorSensor();

#if(__MONITORAR_SERIAL__ == true) 

        if(Boia.LiquidoNoRecipiente() == eLiquidoRecipiente::Cheio) {
            Serial.print(" << Tem agua >>\n");
        } 
        else {
            Serial.print(" << Sem agua >> | Necessário encher o recipiente\n");
        }

#endif


        if(Boia.LiquidoNoRecipiente() == eLiquidoRecipiente::Cheio) 
        {
            // Execurtar rotina de irrigação
#if(__MONITORAR_SERIAL__ == true) 
            Serial.print("\n\tTempo de irrigacao de ");
            Serial.print((int)((__TEMPO_IRRIGACAO_BOMBA__ * 2) / 1000));
            Serial.print(" segundos | Irrigando...");
#endif
            BombaDagua.Irrigar((__TEMPO_IRRIGACAO_BOMBA__ * 2));

#if(__MONITORAR_SERIAL__ == true) 
            Serial.println(" | Irrigado");
#endif
        } else 
        {
            // desenvolver meio de avisar q o recipiente está vazio
#if(__MONITORAR_SERIAL__ == true) 
            Serial.print("\n\t>>> Beep porque o recipiente esta vazio...\n");
#endif

            AlertaSonoro();
        }

    }


#if(__MONITORAR_SERIAL__ == true) 
    Serial.print("\nAguardando 1 minuto para o próximo monitoramento...");
    Serial.print("\n\n");
#endif

    delay(__ESPERA_ENTRE_LEITURA__);

}


void AlertaSonoro() 
{
    tone(pinoBuzzer, 233, 100);
}


// sRecipiente getDadosRecipienteEEPROM()
// {
//     Recipiente.Capacidade = EEPROM.read(EEPROM_Address_CapacidadeRecipiente);
// }

#endif  //Fim da diretiva do código de controle de qual sketch será compilado
