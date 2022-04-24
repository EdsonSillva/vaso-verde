/*
    Classe de uso geral para o projeto Vaso Verde,
    definições, enum's, metodos, etc. que poderão ser
    usados por todas as classes do projeto.  

    Data: 03/2022
    Autor: Edson Silva

*/

#ifndef __USOGERAL_H__
#define __USOGERAL_H__

#define __AGUARDAR_ATIVACAO_SENSOR__     3000
#define __LITROS_POR_MINUTO__             1.5
#define __UM_MINUTO__                   60000
#define __TEMPO_IRRIGACAO_BOMBA__        6000       // 1,5 L/m  |  60000 (1 minuto) = 1,5L | 6000 (6 segundos) = 150ml
#define LIGADO                          true
#define DESLIGADO                       false


/**
 * Enumeration para indicar qual comportamento será adotado no uso do sensor
 */
enum eComportamento {
    SempreLigado,       // Tomar cuidado ao usar esta opção por gerar desgaste prematuro do sensor;
    LigarNaLeitura      // Nesta opção o sensor só vai ser energizado na leitura. Necessário indicar uma porta Digital e é recomendado usar um transistor com fonte de energia externa.
};

/**
 * Enumaration para indicar a umidade da terra do vaso
 */
enum eUmidadeVaso {
    Encharcado,
    Umido,
    Seco
};

/**
 * Enumaration para indicar a quantidade de água no recipiente
 */
enum eLiquidoRecipiente
{
    Cheio,
    Vazio,
    none
};

class UsoGeral
{
private:
    
    
public:
    UsoGeral(/* args */);
    ~UsoGeral();

    void ConfigurarSensorGeral(byte* pinValorSensor, eComportamento* Comportamento, byte* pinLigarSensor, unsigned int* AguardarAtivacao);
    void LigarSensorGeral(bool OnOff, byte* pinLigarSensor, unsigned int* AguardarAtivacao);
    void AguardarLigacaoSensorGeral(int* TempoDoAguardo);
    uint8_t LiquidoUsadoPeloTempo(unsigned int TempoIrrigacao);

};

UsoGeral::UsoGeral() { }
UsoGeral::~UsoGeral() { }

void UsoGeral::ConfigurarSensorGeral(byte* pinValorSensor, eComportamento* Comportamento, byte* pinLigarSensor, unsigned int* AguardarAtivacao) 
{
    pinMode(*pinValorSensor, INPUT);

    if((*Comportamento == eComportamento::LigarNaLeitura) && (*pinLigarSensor != ' ')) 
    {
        pinMode(*pinLigarSensor, OUTPUT);
        LigarSensorGeral(DESLIGADO, pinLigarSensor, AguardarAtivacao);
    }
}

/**
 * Metodo para ligar o sensor ou a bomba d'agua através de uma porta digital
 * 
 * @param OnOff Indica se vai ligar ou desligar a porta digital
 * @param pinLigarSensor pino conectado o sensor ou a bomba d'agua
 * @param AguardarAtivacao informar um tempo que será usado para aguardar o sensor ou a bomba d'agua
 * 
 */
void UsoGeral::LigarSensorGeral(bool OnOff, byte* pinLigarSensor, unsigned int* AguardarAtivacao)
{
    digitalWrite(*pinLigarSensor, OnOff);
    if(OnOff) AguardarLigacaoSensorGeral((int*)AguardarAtivacao);
}

void UsoGeral::AguardarLigacaoSensorGeral(int* TempoDoAguardo) 
{
    delay(*TempoDoAguardo);
}


uint8_t UsoGeral::LiquidoUsadoPeloTempo(unsigned int TempoIrrigacao)
{
    return (uint8_t)(((TempoIrrigacao * __LITROS_POR_MINUTO__) / __UM_MINUTO__) * 100);
}


#endif  // __USOGERAL_H__


