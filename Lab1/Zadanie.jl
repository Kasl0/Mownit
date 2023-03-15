using LinearAlgebra
using CSV
using DataFrames
using Statistics
using Plots

# Funkcja do obliczania iloczynu skalarnego wektorów
function dot_product(x, y)
    return dot(x, y)
end

# Funkcja do mnożenia macierzy kwadratowej przez wektor
function matrix_vector_multiply(A, x)
    return A * x
end

# Tablica zawierająca wszystkie rozmiary wektorów, które będziemy testować
vector_sizes = range(100, stop=10000, step=100)

# Liczba pomiarów
num_measurements = 10

# Struktura DataFrame, w którym będziemy zapisywać wyniki po każdym pomiarze
results = DataFrame(vector_size = Int[], function_name = String[], time = Float64[])

for vector_size in vector_sizes
    
    for i in 1:num_measurements+1 # Wykonujemy n+1 pomiarów, a pierwszy pomiar pomijamy (nie uwzględniamy w wynikach)
        x = rand(vector_size)
        y = rand(vector_size)
        time = (@timed dot_product(x, y)).time
        if i != 1 # Pierwszy pomiar pomijamy
            push!(results, (vector_size, "dot_product", time))
        end
    end

    
    for i in 1:num_measurements+1
        A = rand(vector_size, vector_size)
        x = rand(vector_size)
        time = (@timed matrix_vector_multiply(A, x)).time
        if i != 1
            push!(results, (vector_size, "matrix_vector_multiply", time))
        end
    end
end

# Dane zapisujemy do pliku CSV
CSV.write("results.csv", results)

# Wczytujremy dane z pliku CSV
results = CSV.read("results.csv", DataFrame)

# Korzystając z mechanizmów DataFrame, grupujemy dane po rozmiarze wektora i nazwie funkcji
results_grouped = groupby(results, [:vector_size, :function_name])

# Obliczamy średną i odchylenie standardowe
results_combined = combine(results_grouped, "time" => mean, "time" => var)

# Rysujemy 2 wykresy ze słupkami błędów, podpisanymi osiami i tytułami, wykorzystujemy opcję layout=2
scatter(results_combined.vector_size, results_combined.time_mean, group=results_combined.function_name, yerr=results_combined.time_var, colour=[:red :blue], legend=false, layout=2, xlabel="Rozmiar wektora", ylabel="Czas [s]", title=["Obliczanie iloczynu skalarnego" "Mnożenie macierzy"])

# Uwaga: Dla danych zdefinowanych wyżej, słupki błędu są na tyle małe, że nie widać ich na obrazku
