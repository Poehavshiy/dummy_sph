Вся логика расчета содержится исключительно в Particle.h и в Dummy_flow.cpp Dummy_flow.h
в Dummy_flow.cpp основная функция это calculate(), в ней в свою очередь дергается
    1. calculate_dir(); - расчет производных
    2. calculate_final(); - расчет значений на новом шаге по времени
    Их и надо посмотреть

ссылки на формулы с книжки  приведены в коментария к коду в Dummy_flow.cpp

