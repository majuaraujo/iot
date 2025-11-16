# SmartFocus – Monitoramento de Conforto e Estresse no Ambiente de Trabalho
----
Maria Julia Araujo
RM553384
----
## Contexto e Problema

Com o avanço do **trabalho remoto e híbrido**, cresce a preocupação com o **bem-estar digital** dos profissionais. Longos períodos diante de telas, em ambientes com pouca iluminação ou temperatura inadequada, geram **fadiga, estresse e queda na produtividade**.

Nesse cenário, surge a necessidade de uma solução tecnológica que auxilie na **identificação de condições ambientais desconfortáveis** e incentive pausas saudáveis — alinhando tecnologia IoT e qualidade de vida no trabalho.

---

## Solução Desenvolvida

O **SmartFocus** é um sistema IoT que monitora o ambiente de trabalho utilizando sensores de **luminosidade (LDR)** e **temperatura/umidade (DHT22)**.  
Com base nos dados coletados, o ESP32 interpreta o **nível de conforto e estresse** do usuário e aciona **alertas visuais e sonoros** quando o ambiente se torna inadequado (por exemplo, pouca luz ou calor excessivo).

Além disso, os dados são **enviados via protocolo MQTT** para um **broker público (HiveMQ)**, permitindo o acompanhamento remoto e a integração com dashboards ou automações futuras.

---

## Componentes Utilizados

| Componente | Função |
|-------------|--------|
| **ESP32** | Microcontrolador com Wi-Fi integrado. |
| **Sensor DHT22** | Mede temperatura e umidade do ambiente. |
| **Sensor LDR** | Detecta variações na luminosidade. |
| **LED Vermelho** | Indica condição de desconforto. |
| **Buzzer** | Emite alerta sonoro em situações críticas. |
| **Resistor 220 Ω** | Protege o LED. |

---

## Esquema do Circuito

<img width="613" height="412" alt="image" src="https://github.com/user-attachments/assets/04dd4f6e-42eb-40a4-9cc2-1223361d20a0" />

---

## Conectividade MQTT

O sistema se comunica com o **broker público HiveMQ** para envio das leituras em tempo real.

- **Broker:** `broker.hivemq.com`  
- **Porta:** `1883`  
- **Tópico utilizado:**


## Exemplo de mensagem publicada:
```json
{
"temperatura": 28.5,
"umidade": 63.2,
"luz": 480,
"nivelEstresse": 72,
"classificacaoEstresse": "Alto",
"status": "Estresse alto - sugerir pausa"
}
```
---

## Funcionamento do Código:

1. O ESP32 conecta-se à rede Wi-Fi (SSID: Wokwi-GUEST).
2. Lê os valores de temperatura, umidade e luminosidade.
3. Calcula o nível de estresse estimado, considerando calor, umidade e luminosidade.
4. Caso ultrapasse o limite de conforto:
5. Acende o LED vermelho;
6. Ativa o buzzer com sinal sonoro;
7. Publica o estado via MQTT.
8. O sistema também monitora o tempo de tela e recomenda pausas após longos períodos de uso contínuo.

## Dependências e Bibliotecas:
1. Acesse o projeto no Wokwi: https://wokwi.com/projects/447750342571871233
2. Clique em “Start Simulation”.
3. Acompanhe o console serial e observe as mensagens MQTT.
4. Conecte-se ao broker HiveMQ:
    Acesse https://www.hivemq.com/demos/websocket-client/
    Clique em Connect
    Em Topic, digite: smartfocus/status
    Clique em Subscribe
    Veja as mensagens JSON chegando em tempo real

## Resultados Esperados:

- Alertas imediatos de desconforto térmico ou visual.
- Monitoramento remoto via MQTT para análises de bem-estar digital.
- Conscientização sobre a importância das pausas e ergonomia no futuro do trabalho.
- Potencial de expansão para integrar sensores de postura, ruído ou até wearables (smartwatch).

## Impacto e ODS:

Este projeto contribui para os Objetivos de Desenvolvimento Sustentável (ODS) da ONU:

- ODS 3 – Saúde e Bem-Estar
  Promove hábitos saudáveis e pausas regulares no trabalho.

- ODS 9 – Indústria, Inovação e Infraestrutura
  Estimula o uso da tecnologia IoT para ambientes mais inteligentes e sustentáveis.

  
