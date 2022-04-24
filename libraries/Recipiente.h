/*
    Classe para conter o código de controle do recipiente uado para 
    armazemar a água usada na irrigação do solo

    Data: 03/2022
    Autor: Edson Silva

*/

#ifndef __RECIPIENTE_H__
#define __RECIPIENTE_H__

#ifndef __USOGERAL_H__
#include "UsoGeral.h"
#endif

/*******************************************************************************
 *  DEFINIÇÃO - Classe que controla a interface com o recepiente uado para 
 *              armazemar a água usada
 *******************************************************************************/

class Recipiente : protected MiniBombaDagua, protected SensorBoia
{
private:
    uint8_t                     _Capacidade;        // 100 = 1 litro
    uint8_t                     _JaUsado;
    SensorBoia*                 _Boia;
    MiniBombaDagua*             _BombaDagua;
    
public:
    Recipiente();
    ~Recipiente();

    void Iniciar(uint8_t Capacidade, SensorBoia *SensorBoiaRecipiente, MiniBombaDagua *MiniBombaDaguaRecipiente);
    void getDadosRecipienteEEPROM();
    void getDadosRecipiente(uint8_t Capacidade, uint8_t JaUsado, uint8_t Disponivel);
    void setDadosRecipiente(uint8_t Capacidade, uint8_t JaUsado);

};

/*****************************************************************************
 *  METODOS - Classe que controla a interface com o sensor de umidade de solo
 *****************************************************************************/

Recipiente::Recipiente() { }
Recipiente::~Recipiente() { }


/** 
 * Configura a classe do Recepiente informando a capacidade em litros, o sensor da boia e a Mini bomba d'agua.
 * 
 * @param Capacidade Informar a capacidade em litros 
 * @param SensorBoiaRecipiente Ponteiro para Classe contendo o controle do sensor da boia
 * @param MiniBombaDaguaRecipiente Ponteiro para Classe contendo o controle da bomba d'agua
 */
void Recipiente::Iniciar(uint8_t Capacidade, SensorBoia *SensorBoiaRecipiente, MiniBombaDagua *MiniBombaDaguaRecipiente)
{
    _Capacidade   = Capacidade;
    _Boia = SensorBoiaRecipiente;
    _BombaDagua = MiniBombaDaguaRecipiente;

}

void Recipiente::setDadosRecipiente(uint8_t Capacidade, uint8_t JaUsado)
{
    _Capacidade   = Capacidade;
    _JaUsado = JaUsado;
}

void Recipiente::getDadosRecipiente(uint8_t Capacidade, uint8_t JaUsado, uint8_t Disponivel)
{
    Capacidade = _Capacidade;
    JaUsado = _JaUsado;
    Disponivel = _Capacidade - _JaUsado;
}


#endif  // __RECIPIENTE_H__

