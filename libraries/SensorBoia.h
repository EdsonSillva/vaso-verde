/*
    Classe para conter o código de controle do sensor que irá dentro 
    do recepiente de água para verificar o nível d'agua.   
    
    Data: 03/2022
    Autor: Edson Silva

*/

#ifndef __SENSORBOIA_H__
#define __SENSORBOIA_H__

#ifndef __USOGERAL_H__
#include "UsoGeral.h"
#endif

/*******************************************************************************
 *  DEFINIÇÃO - Classe que controla a interface com o sensor boia d'agua
 *******************************************************************************/
//
class SensorBoia
{

private:

    byte                            _pinValorSensor;
    byte                            _pinLigarSensor;
    bool                            _ValorSensor = false;
    eComportamento                  _Comportamento = eComportamento::LigarNaLeitura;
    eLiquidoRecipiente              _LiquidoRecipiente = eLiquidoRecipiente::none;
    unsigned int                    _AguardarAtivacao = __AGUARDAR_ATIVACAO_SENSOR__;

    UsoGeral                        _UGeral;

    bool LerSensor();

public:

    static volatile bool            ValorSensorMudou;

    SensorBoia();
    ~SensorBoia();

    void Iniciar(byte PinoAnalogico, eComportamento Comportamento, byte PinoDigitalLigarSensor);               
    void setComportamento(eComportamento Comportamento);
    eComportamento getComportamento();
    void setTempoAtivarSensor(unsigned int Tempo);
    void LigarSensor(unsigned int AguardarAtivacao);
    void DesligarSensor();
    bool getValorSensor();
    eLiquidoRecipiente LiquidoNoRecipiente();
    String LiquidoNoRecipienteDescricao();
    byte getPinoValorSensor();
    void ISR_Boia_Change();

};

// Definição de variáveis staticas 

volatile bool SensorBoia::ValorSensorMudou;

/*****************************************************************************
 *  METODOS - Classe que controla a interface com o sensor boia d'agua
 *****************************************************************************/

SensorBoia::SensorBoia()  { }
SensorBoia::~SensorBoia() { }

#pragma region Metodos Privados

bool SensorBoia::LerSensor() 
{ 
    bool ValorLido = false;
    ValorSensorMudou = false;

// #if(__MONITORAR_SERIAL__ == true) 
//     Serial.print("Entrei na rotina LerSensor() - Boia");
//     Serial.print(" | _Comportamento: "),     Serial.print(_Comportamento),     Serial.print("\n");
// #endif

    if(_Comportamento == eComportamento::LigarNaLeitura) 
        _UGeral.LigarSensorGeral(LIGADO, &_pinLigarSensor, &_AguardarAtivacao);      // Liga o sensor
        
    ValorLido = (bool)digitalRead(_pinValorSensor); 

// #if(__MONITORAR_SERIAL__ == true) 
//     Serial.print("Valor do sensor Boia: ( ");
//     Serial.print(ValorLido),     Serial.print(" ) | _pinValorSensor("),     Serial.print(_pinValorSensor);
//     Serial.print(")"),     Serial.print("\n");
// #endif

    if(ValorLido != _ValorSensor) SensorBoia::ValorSensorMudou = true;
    _ValorSensor = ValorLido; 

    if(_Comportamento == eComportamento::LigarNaLeitura) 
        _UGeral.LigarSensorGeral(DESLIGADO, &_pinLigarSensor, &_AguardarAtivacao);   // Desliga o sensor

    return _ValorSensor; 
}

#pragma endregion Metodos Privados

#pragma region Medodos Publicos

/** 
 * Configura a classe do sensor informando o pino digital a ser usado, o comportamento 
 * que o sensor irá adotar para trabalhar e se o comportamento for <LigarNaLeitura> 
 * será necessário informar um pino digital que será configurado como saída (Ex.: pinMode(PinoDigitalLigarSensor, OUTPUT)).
 * 
 * 
 * @param PinoDigitalValor Informar um pino com suporte digital
 * @param Comportamento Informar qual será o comportamento na leitura do sensor: Default LigarNaLeitura
 * @param PinoDigitalLigarSensor Informar um pino digital somente se o comportamento for <LigarNaLeitura>. Para o comportamento 
 *                               <SempreLigado> assume-se que o sensor ficará ligado direto na fonte de energia.
 * 
 */
void SensorBoia::Iniciar(byte PinoDigitalValor, eComportamento Comportamento = eComportamento::LigarNaLeitura, byte PinoDigitalLigarSensor = ' ')
{
    _pinValorSensor =  PinoDigitalValor;
    _Comportamento = Comportamento;
    _pinLigarSensor = PinoDigitalLigarSensor;

    _UGeral.ConfigurarSensorGeral(&_pinValorSensor, &_Comportamento, &_pinLigarSensor, &_AguardarAtivacao);
}

/**
 * Indica o comportamento de leitura do sensor
 * 
 * @param Comportamento Informar qual será o comportamento na leitura do sensor que pode ser SempreLigado ou LigarNaLeitura
 * 
 * */
void SensorBoia::setComportamento(eComportamento Comportamento)
{
    _Comportamento = Comportamento;
}

/**
 * Retorna o comportamento de leitura do sensor
 * 
 * */
eComportamento SensorBoia::getComportamento()
{
    return _Comportamento;
}

/**
 * Indica o tempo que o sensor vai aguardar após ligar o sensor para liberar para leitura
 * 
 * @param Tempo indicar o tempo em milisegundos
 *  
 */
void SensorBoia::setTempoAtivarSensor(unsigned int Tempo)
{
    _AguardarAtivacao = Tempo;
}

/**
 * Liga o sensor para habilitar a medição 
 */
void SensorBoia::LigarSensor(unsigned int AguardarAtivacao)
{
    _UGeral.LigarSensorGeral(true, &_pinLigarSensor, &AguardarAtivacao);
}

void SensorBoia::DesligarSensor()
{
    _UGeral.LigarSensorGeral(false, &_pinLigarSensor, 0);
}

/**
 * Ler o valor do sensor
 * 
 * @return Valor lido do sensor 
 */ 
bool SensorBoia::getValorSensor() 
{
    
    bool ValorSensor = LerSensor();
    if(ValorSensor) {  _LiquidoRecipiente = eLiquidoRecipiente::Cheio;     }
    else            {  _LiquidoRecipiente = eLiquidoRecipiente::Vazio;     }
    
    return ValorSensor;
}

byte SensorBoia::getPinoValorSensor()
{
    return _pinValorSensor;
}

void SensorBoia::ISR_Boia_Change()
{
    ValorSensorMudou = true;
}

/**
 * Mostra se a boia está indicando cheio ou vazio
 * Se não for possível fazer a leitura do sensor
 * será retornado "none" indicando sem leitura
 * 
 * @return Valor do emun correspondente
 */ 
eLiquidoRecipiente SensorBoia::LiquidoNoRecipiente()
{
    return _LiquidoRecipiente;
}

/**
 * Mostra se a boia está indicando cheio ou vazio
 * Se não for possível fazer a leitura do sensor
 * será retornado "Sem leitura"
 * 
 * @return A descrição do emun correspondente
 */ 
String SensorBoia::LiquidoNoRecipienteDescricao()
{
    if(_LiquidoRecipiente == eLiquidoRecipiente::Cheio)
    {
        return F("Cheio");

    } else if(_LiquidoRecipiente == eLiquidoRecipiente::Vazio) 
    {
        return F("Vazio");

    } else 
    {
        return "sem leitura";
    }
}

#pragma endregion Medodos Publicos

#endif


