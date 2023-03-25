library("tidyverse")

# Wczytujemy dane z pliku CSV
results = read.csv("results.csv")

# Grupujemy dane po rozmiarze wektora i nazwie funkcji
avg_results <- aggregate(time ~ vector_size:function_name, data=results, FUN=mean)

# Dodajemy kolumnę z odchyleniem standardowym
avg_results$sd <- aggregate(time ~ vector_size:function_name, data=results, FUN=sd)$time

# Filtrujemy dane po nazwie funkcji i rozdzielamy do dwóch osobnych tabeli
avg_results_dot_product <- avg_results[avg_results$function_name=="dot_product",]
avg_results_matrix_vector_multiply <- avg_results[avg_results$function_name=="matrix_vector_multiply",]

# Rysujemy 2 wykresy ze słupkami błędów, podpisanymi osiami i tytułami
ggplot(avg_results_dot_product, aes(vector_size, time)) + geom_point() + geom_errorbar(aes(ymin = time-sd, ymax = time+sd, width=.25)) + ylab("Czas [s]") + xlab("Rozmiar wektora") + ggtitle("Obliczanie iloczynu skalarnego")
ggplot(avg_results_matrix_vector_multiply, aes(vector_size, time)) + geom_point() + geom_errorbar(aes(ymin = time-sd, ymax = time+sd, width=.25)) + ylab("Czas [s]") + xlab("Rozmiar wektora") + ggtitle("Mnożenie macierzy")
