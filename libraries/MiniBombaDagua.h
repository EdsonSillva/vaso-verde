/*
    Classe para conter o código de controle da mini bomba d'agua que irá dentro 
    do recepiente de água para irrigar o vaso da planta.   
    
    Data: 03/2022
    Autor: Edson Silva

*/

#ifndef __MINIBOMBADAGUA_H__
#define __MINIBOMBADAGUA_H__

#ifndef __USOGERAL_H__
#include "UsoGeral.h"
#endif

/*******************************************************************************
 *  DEFINIÇÃO - Classe que controla a interface com a bomba d'agua
 *******************************************************************************/
//
class MiniBombaDagua
{
private:

    byte                        _pinLigarBombaDagua;
    unsigned int                _TempoIrrigacao = __TEMPO_IRRIGACAO_BOMBA__;
    uint8_t*                    _Capacidade;
    uint8_t*                    _JaUsado;
    SensorBoia*                 _Boia;      
    bool                        _RecipienteComAgua = true;
    UsoGeral                    _UGeral;


public:
    MiniBombaDagua();
    // {
    // salvar a capacidade e uso do recepiente na EEPROM
    // }
    ~MiniBombaDagua();

    void Iniciar(byte PinoDigitalLigarBomba, SensorBoia* Boia);    
    void Irrigar(unsigned int TempoIrrigacao);

protected:

    void setDadosRecipiente(uint8_t* Capacidade, uint8_t* JaUsado);

};


/*****************************************************************************
 *  METODOS - Classe que controla a interface com a bomba d'agua
 *****************************************************************************/

MiniBombaDagua::MiniBombaDagua() { }            // Construtor
MiniBombaDagua::~MiniBombaDagua() { }           // Destrutor

void ISR_Boia_CHANGE()
{
    SensorBoia::ValorSensorMudou = true;    
}

#pragma region Metodos Privados


#pragma endregion Metodos Privados

#pragma region Medodos Publicos

// void (*MiniBombaDagua::ISR_TASK)(void)
// {  };

/** 
 * Configura a classe da bomba d'agua informando o pino digital a ser usado, o comportamento 
 * que o sensor irá adotar para trabalhar e se o comportamento for <LigarNaLeitura> 
 * será necessário informar um pino digital que será configurado como saída (Ex.: pinMode(PinoDigitalLigarSensor, OUTPUT)).
 * 
 * 
 * @param PinoDigitalLigarSensor Informar um pino digital somente se o comportamento for <LigarNaLeitura>. Para o comportamento 
 *                               <SempreLigado> assume-se que o sensor ficará ligado direto na fonte de energia.
 * @param Boia                   Ponteiro para o objeto SensorBoia que será usado no controle do uso da bomba d'agua
 * 
 */
void MiniBombaDagua::Iniciar(byte PinoDigitalLigarSensor, SensorBoia* Boia)
{
    _pinLigarBombaDagua = PinoDigitalLigarSensor;

    pinMode(_pinLigarBombaDagua, OUTPUT);
    
    _UGeral.LigarSensorGeral(DESLIGADO, &_pinLigarBombaDagua, 0);

    _Boia = Boia;

}

/** 
 * Recebe o ponteiro para as variáveis de controle da Capacidade e JaUsado do recepiente
 * 
 * @param Capacidade  Ponteiro para a informação da capacidade do recepiente
 * @param Jausado     Ponteiro para a informação de Jausado do recepiente
 * 
 */
void MiniBombaDagua::setDadosRecipiente(uint8_t* Capacidade, uint8_t* JaUsado)
{
    _Capacidade = Capacidade;
    _JaUsado = JaUsado;
}

/**
 * Aciona a bomda d'agua pelo tempo indicado
 * 
 * @param TempoIrrigacao Indica o tempo que a bomba d'agua ficara ligada
 * 
 */
void MiniBombaDagua::Irrigar(unsigned int TempoIrrigacao = __TEMPO_IRRIGACAO_BOMBA__)
{
    
    eComportamento BoiaComportamentoAtual =  _Boia->getComportamento();
    _Boia->setComportamento(eComportamento::SempreLigado);
    _Boia->LigarSensor(10);
    _RecipienteComAgua = _Boia->getValorSensor();


#if(__MONITORAR_SERIAL__ == true) 
        Serial.print("\n\t\tValor Sensor Boia: "), Serial.println(_Boia->getValorSensor());
        // Serial.print("\n\t\tAttachando a rotina no pino:"), Serial.println(_Boia->getPinoValorSensor());
#endif

    if(_RecipienteComAgua)
    {
        _Boia->ValorSensorMudou = false;
        //delay(4);      

        attachInterrupt(digitalPinToInterrupt(_Boia->getPinoValorSensor()), ISR_Boia_CHANGE, LOW);

        _UGeral.LigarSensorGeral(LIGADO, &_pinLigarBombaDagua, 0);  // Ligando a Bomba D'agua 

        unsigned long TempoIrrigando = millis() + TempoIrrigacao;


#if(__MONITORAR_SERIAL__ == true) 
    Serial.print("TempoIrrigacao: ");
    Serial.print(TempoIrrigacao);
    Serial.print(" | ");
    Serial.print("TempoIrrigando: ");
    Serial.print(TempoIrrigando);

    Serial.print(" | ");
    Serial.print("_RecipienteComAgua: ");
    Serial.print(_RecipienteComAgua);
    Serial.print(" | ");
    Serial.print("millis(): ");
    Serial.print(millis());
    Serial.print("\n");
#endif

        int Segundos = 1000;
        unsigned long UmSegundo = 0;

        while (_RecipienteComAgua && TempoIrrigando > millis())
        {
            
            if(UmSegundo <= millis()) 
            {
                // Serial.print(".");        
                UmSegundo = millis() + 1000;
                // Serial.print(millis()), Serial.print(", ");
                Serial.print((Segundos / 1000)), Serial.print(", ");
                Segundos = Segundos + 1000;
            }

            if(_Boia->ValorSensorMudou) {

#if(__MONITORAR_SERIAL__ == true) 
    Serial.print("\t\tRecipiente sem água... irrigação interrompida\n");
#endif             
      
                _RecipienteComAgua = false;
            }
    //Serial.print(".");

        }

        Serial.print(" >> "), Serial.print(UmSegundo), Serial.print(" << ");


    // Serial.print("\nmillis(): ");
    // Serial.print(millis());
    // Serial.print("\n");


        _UGeral.LigarSensorGeral(DESLIGADO, &_pinLigarBombaDagua, 0);

        detachInterrupt(digitalPinToInterrupt(_Boia->getPinoValorSensor()));

        if(_RecipienteComAgua)
           *_JaUsado = _UGeral.LiquidoUsadoPeloTempo(TempoIrrigacao);
        else
           *_JaUsado = *_Capacidade;

    }

    _Boia->DesligarSensor();                                // Desliga o sensor Boia
    _Boia->setComportamento(BoiaComportamentoAtual);        // Restaura o comportamento do sensor boia

}

#pragma endregion Medodos Publicos

#endif  // __MINIBOMBADAGUA_H__