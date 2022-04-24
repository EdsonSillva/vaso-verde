/*
    Classe para conter o código de controle do sensor que irá dentro do vaso 
    para medir a umidade do solo.

    Data: 03/2022
    Autor: Edson Silva

*/

#ifndef __SENSORVASO_H__
#define __SENSORVASO_H__

#ifndef __USOGERAL_H__
#include "UsoGeral.h"
#endif

/*******************************************************************************
 *  DEFINIÇÃO - Classe que controla a interface com o sensor de umidade de solo
 *******************************************************************************/

class SensorVaso 
{
private:

    byte                _pinValorSensor;
    byte                _pinLigarSensor;
    int                 _ValorSensor;
    eComportamento      _Comportamento = eComportamento::LigarNaLeitura;
    unsigned int        _AguardarAtivacao = __AGUARDAR_ATIVACAO_SENSOR__;

    void ConfigurarSensor();
    void LigarSensor(bool OnOff); 
    void AguardarLigacaoSensor(int TempoDoAguardo);
    int LerSensor();

public:

    SensorVaso();
    ~SensorVaso();

    void Iniciar(byte PinoAnalogico, eComportamento Comportamento, byte PinoDigitalLigarSensor);               
    void setComportamento(eComportamento Comportamento);
    void setTempoAtivarSensor(unsigned int Tempo);
    int getValorSensor();
    eUmidadeVaso getEstadoDoSolo();
    String getEstadoDoSoloDescricao();

}; // Fim da Classe

/*****************************************************************************
 *  METODOS - Classe que controla a interface com o sensor de umidade de solo
 *****************************************************************************/

SensorVaso::SensorVaso()  { }                   // Construtor
SensorVaso::~SensorVaso() { }                   // Destrutor

#pragma region Metodos Privados

void SensorVaso::ConfigurarSensor() 
{
    pinMode(_pinValorSensor, INPUT);

    if((_Comportamento == eComportamento::LigarNaLeitura) && (_pinLigarSensor != ' ')) 
    {
        pinMode(_pinLigarSensor, OUTPUT);
        LigarSensor(LOW);
    }
}

void SensorVaso::LigarSensor(bool OnOff)
{
    
    // Serial.print("Entrei na rotina LigarSensor()");
    // Serial.print(" | _pinLigarSensor= "),     Serial.print(_pinLigarSensor);
    // Serial.print(" | OnOff="),                Serial.print(OnOff) ,     Serial.print("\n");


    digitalWrite(_pinLigarSensor, OnOff);
    if(OnOff) AguardarLigacaoSensor(_AguardarAtivacao);

    // Serial.println("Saindo da rotina LigarSensor()");

}

void SensorVaso::AguardarLigacaoSensor(int TempoDoAguardo = __AGUARDAR_ATIVACAO_SENSOR__) 
{
    
    // Serial.print("Entrei na rotina AguardarLigacaoSensor()");
    // Serial.print(" |TempoDoAguardo= "),    Serial.print(TempoDoAguardo),     Serial.print("\n");

    delay(TempoDoAguardo);

    // Serial.println("Saindo da rotina AguardarLigacaoSensor()");


}

int SensorVaso::LerSensor() 
{ 

    // Serial.print("Entrei na rotina LerSensor()");
    // Serial.print(" | _Comportamento: "),     Serial.print(_Comportamento),     Serial.print("\n");


    if(_Comportamento == eComportamento::LigarNaLeitura) 
    {
        // Serial.print("   Ligando Sensor...\n");
        LigarSensor(HIGH);      // Liga o sensor
    }

    // Serial.print("   Lendo Sensor...\n");
    _ValorSensor = analogRead(_pinValorSensor);
    // Serial.print("   Sensor lido\n");


    if(_Comportamento == eComportamento::LigarNaLeitura) 
    {
        // Serial.print("   Desligando Sensor...\n");
        LigarSensor(LOW);       // Desliga o sensor
    }
    // Serial.println("Saindo da rotina LerSensor()");


    return _ValorSensor; 
}

#pragma endregion Metodos Privados

#pragma region Medodos Publicos

/** 
 * Configura a classe do sensor informando o pino analógico a ser usado, o comportamento 
 * que o sensor irá adotar para trabalhar e se o comportamento for <LigarNaLeitura> 
 * será necessário informar um pino digital que será configurado como saída (Ex.: pinMode(PinoDigitalLigarSensor, OUTPUT)).
 * 
 * 
 * @param PinoAnalogicoValor Informar um pino com suporte analógico
 * @param Comportamento Informar qual será o comportamento na leitura do sensor: Default LigarNaLeitura
 * @param PinoDigitalLigarSensor Informar um pino digital somente se o comportamento for <LigarNaLeitura>. Para o comportamento 
 *                               <SempreLigado> assume-se que o sensor ficará ligado direto na fonte de energia.
 * 
 */
void SensorVaso::Iniciar(byte PinoAnalogicoValor, eComportamento Comportamento = eComportamento::LigarNaLeitura, byte PinoDigitalLigarSensor = ' ')
{
    _pinValorSensor = PinoAnalogicoValor;
    _Comportamento = Comportamento;
    _pinLigarSensor = PinoDigitalLigarSensor;

    ConfigurarSensor();
}

/**
 * Indica o comportamento de leitura do sensor
 * 
 * @param Comportamento Informar qual será o comportamento na leitura do sensor que pode ser SempreLigado ou LigarNaLeitura
 * 
 * */
void SensorVaso::setComportamento(eComportamento Comportamento)
{
    _Comportamento = Comportamento;
}

/**
 * Indica o tempo que o sensor vai aguardar após ligar o sensor para liberar para leitura
 * 
 * @param Tempo indicar o tempo em milisegundos
 *  
 */
void SensorVaso::setTempoAtivarSensor(unsigned int Tempo) 
{
    _AguardarAtivacao = Tempo;
}

/**
 * Ler o valor do sensor
 * 
 * @return Valor lido do sensor
 */ 
int SensorVaso::getValorSensor() 
{

    int ValorSensor;

    // Serial.println("Entrei na rotina getValorSensor");
 
    ValorSensor = LerSensor();

    // Serial.println("Saindo da rotina getValorSensor");

//    return LerSensor();
    return ValorSensor;

}

/**
 * Mostra estado do solo baseado no valor lido do sensor
 * 
 * @return Valor do emun correspondente
 */ 
eUmidadeVaso SensorVaso::getEstadoDoSolo()
{
    if (_ValorSensor > 800)
    {
        return eUmidadeVaso::Seco;
    } else if(_ValorSensor > 400)
    {
        return eUmidadeVaso::Umido;
    } else 
    {
        return eUmidadeVaso::Encharcado;
    }
    
}

/**
 * Mostra estado do solo baseado no valor lido do sensor
 * 
 * @return Descrição do enum correspondente
 */ 
String SensorVaso::getEstadoDoSoloDescricao()
{
    if (_ValorSensor > 800)
    {
        return F("Seco");
    } else if(_ValorSensor > 400)
    {
        return F("Umido");
    } else 
    {
        return F("Encharcado");
    }
    
}


#pragma endregion Metodos Publicos

#endif  // __SENSORVASO_H__
