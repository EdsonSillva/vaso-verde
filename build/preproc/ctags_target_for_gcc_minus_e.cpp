# 1 "c:\\Users\\edson\\OneDrive\\Documentos\\Arduino_vscode\\VasoVerde\\VasoVerde.ino"

/*

    Projeto Vaso Verde

    Descrição: Este projeto tem como objetivo cuidar da irrigação do solo de um vaso com planta.

               Serão utilizados um sensor de umidade de solo, um sensor com bóia e uma mini bomba d'agua.

    

    Autor: Edson Silva

    Data: 03/2022



*/
# 12 "c:\\Users\\edson\\OneDrive\\Documentos\\Arduino_vscode\\VasoVerde\\VasoVerde.ino"
/*

    Define para direcionar a compilação do sketch

    __PROJETO_EM_BRANCO__ true  : Compila um sketch em branco

    __PROJETO_EM_BRANCO__ false : Complia o projeto desenvolvido



*/







# 31 "c:\\Users\\edson\\OneDrive\\Documentos\\Arduino_vscode\\VasoVerde\\VasoVerde.ino" 2
# 43 "c:\\Users\\edson\\OneDrive\\Documentos\\Arduino_vscode\\VasoVerde\\VasoVerde.ino"
SensorVaso Vaso;
SensorBoia Boia;
MiniBombaDagua BombaDagua;
Recipiente RecipienteDagua;

int ValorSensorVaso = 0x0;
bool ValorSensorBoia = false;


/**

 * Definição das funções utilizadas no projeto

 */
# 56 "c:\\Users\\edson\\OneDrive\\Documentos\\Arduino_vscode\\VasoVerde\\VasoVerde.ino"
void setup();
void loop();
void AlertaSonoro();
//sRecipiente getDadosRecipienteEEPROM();



void setup()
{
    // Inicializa os Sensores
    // ***********************
    Vaso.Iniciar(A0, eComportamento::LigarNaLeitura, 10);
    Boia.Iniciar(03 /* Pino com interrupção*/, eComportamento::LigarNaLeitura, 13);
    BombaDagua.Iniciar(12, &Boia);

    RecipienteDagua.Iniciar(100 /* 100 = 1 litro*/, &Boia, &BombaDagua);

    //Iniclializa pino Buzzer
    pinMode(05, 0x1);

    //Colocar a leitura da EEPROM para recuperar o valor da capacidade do recipiente e seu uso



    Serial.begin(9600);
    while(!Serial);
    Serial.print("Monitor serial habilitado - Projeto Vaso Verde\n");



}

void loop()
{


    Serial.println("Iniciando monitoramento...\n");


    ValorSensorVaso = Vaso.getValorSensor();

// #if(__MONITORAR_SERIAL__ == true) 
//     Serial.println("Voltando da leitura do sensor");
//     Serial.print("ValorSensorVaso=");
//     Serial.println(ValorSensorVaso);
// #endif


    Serial.print("Estado do solo do vaso: ");
    Serial.print(Vaso.getEstadoDoSoloDescricao());
    Serial.print(" | ");
    Serial.println(ValorSensorVaso);


    if(Vaso.getEstadoDoSolo() == eUmidadeVaso::Seco)
    {
        // TODO: Verificar se tem água no recipiente
        // TODO: Acionar a bomba de água


            Serial.println("\tNecessário irrigar o solo");
            Serial.print("\t\tVerificando se o recipiente tem agua ... ");

        Boia.getValorSensor();



        if(Boia.LiquidoNoRecipiente() == eLiquidoRecipiente::Cheio) {
            Serial.print(" << Tem agua >>\n");
        }
        else {
            Serial.print(" << Sem agua >> | Necessário encher o recipiente\n");
        }




        if(Boia.LiquidoNoRecipiente() == eLiquidoRecipiente::Cheio)
        {
            // Execurtar rotina de irrigação

            Serial.print("\n\tTempo de irrigacao de ");
            Serial.print((int)((6000 /* 1,5 L/m  |  60000 (1 minuto) = 1,5L | 6000 (6 segundos) = 150ml*/ * 2) / 1000));
            Serial.print(" segundos | Irrigando...");

            BombaDagua.Irrigar((6000 /* 1,5 L/m  |  60000 (1 minuto) = 1,5L | 6000 (6 segundos) = 150ml*/ * 2));


            Serial.println(" | Irrigado");

        } else
        {
            // desenvolver meio de avisar q o recipiente está vazio

            Serial.print("\n\t>>> Beep porque o recipiente esta vazio...\n");


            AlertaSonoro();
        }

    }



    Serial.print("\nAguardando 1 minuto para o próximo monitoramento...");
    Serial.print("\n\n");


    delay(60000 /* 1 minuto*/);

}


void AlertaSonoro()
{
    tone(05, 233, 100);
}


// sRecipiente getDadosRecipienteEEPROM()
// {
//     Recipiente.Capacidade = EEPROM.read(EEPROM_Address_CapacidadeRecipiente);
// }
