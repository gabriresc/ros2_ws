# Composable node básico (ROS 2 Jazzy)

Este exemplo publica uma mensagem `std_msgs/msg/String` no tópico `/mensagem`
a cada segundo e mostra um contador no terminal.

Um composable node é um nó que pode ser carregado como biblioteca dentro de
um processo chamado **container**. Vários componentes podem compartilhar esse
processo; neste exemplo, carregamos apenas um para mostrar o básico.

## Compilar e executar

Na raiz do workspace:

```bash
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash
colcon build --packages-select composable_basico --symlink-install
source install/setup.bash
ros2 launch composable_basico composable_basico.launch.py
```

Em outro terminal, carregue o ambiente e veja as mensagens e o componente:

```bash
cd ~/ros2_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 topic echo /mensagem
```

Interrompa o `echo` com `Ctrl+C` e consulte os componentes carregados:

```bash
ros2 component list
```

Você deverá ver `/meu_container` contendo `/meu_componente`. Use `Ctrl+C` no
terminal do launch para encerrar o container e o componente.

## O que observar no código

1. Em `src/meu_componente.cpp`, a classe herda de `rclcpp::Node`, como um nó comum.
2. O construtor recebe `const rclcpp::NodeOptions &` e repassa essas opções ao nó.
3. `RCLCPP_COMPONENTS_REGISTER_NODE` torna a classe carregável pelo container.
4. No `CMakeLists.txt`, `add_library(... SHARED ...)` cria a biblioteca e
   `rclcpp_components_register_node` registra o plugin no ROS 2.
5. O launch inicia o container e escolhe qual classe ele deve carregar pelo nome
   do `plugin`, que precisa corresponder ao nome registrado em C++ e no CMake.

O componente não tem `main()`: o container gerencia sua execução. O argumento
`EXECUTABLE` no CMake também gera um executável para experimentar o mesmo nó
sozinho, sem iniciar o launch:

```bash
ros2 run composable_basico meu_componente_node
```

Execute essa alternativa após encerrar o launch, para evitar dois nós com o
mesmo nome publicando no mesmo tópico.

## Carregar manualmente (opcional)

Para enxergar as duas etapas que o launch automatiza, após encerrá-lo execute
em um terminal com o ambiente carregado:

```bash
ros2 run rclcpp_components component_container --ros-args -r __node:=meu_container
```

Em outro terminal com o ambiente carregado, adicione o componente ao processo:

```bash
ros2 component load /meu_container composable_basico composable_basico::MeuComponente
ros2 component list
```
