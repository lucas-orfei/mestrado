import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

file_path = "./photon/OUT400152_tratado"
num_simulacoes = 316  # Número de simulações
r_max = 1000           # Distância máxima para análise
num_bins = 100         # Número de bins no histograma

# Leitura dos dados
with open(file_path, "r") as file:
    dados = np.loadtxt(file, usecols=(3,))  # Lê apenas a 4ª coluna (r_p)

# Filtra valores dentro do intervalo e remove NaNs
r_p_values = dados[~np.isnan(dados) & (dados <= r_max)]

# Criação do histograma sem pesos
bin_edges = np.linspace(0, r_max, num_bins + 1)
counts, _ = np.histogram(r_p_values, bins=bin_edges)

# Cálculo da densidade correta
bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2    # Centros dos bins
delta_r = bin_edges[1] - bin_edges[0]                 # Largura do bin
areas = 2 * np.pi * bin_centers * delta_r             # Área de cada anel
density = counts / (num_simulacoes * areas)           # Densidade normalizada

# Plot do histograma corrigido
fig, ax = plt.subplots(figsize=(8, 8), facecolor='#FFEBD6')
ax.hist(bin_centers, bins=bin_edges, weights=density, 
        histtype='step', color='#29339B', label="Primary Photon 1E15 eV")
ax.set(xlabel='r (cm)', ylabel=r'$\rho$ (partículas cm$^{-2}$)',
       yscale='log')  
ax.legend()

def modelo_teorico(r, C, s, r0=7800):
    return C/(r0**2) * (r/r0)**s

# Ajuste da curva apenas para bins com dados
mascara = density > 0  # Ignora bins vazios
xdata = bin_centers[mascara]
ydata = density[mascara]

try:
    popt, pcov = curve_fit(modelo_teorico, xdata, ydata,
                           p0=[1E3, -0.5],  
                           bounds=([1, -1], [np.inf, 2]),  
                           maxfev=5000)

    x_fit = np.linspace(1, r_max, 500)
    y_fit = modelo_teorico(x_fit, *popt)
    ax.plot(x_fit, y_fit, color='#F95738', label="Ajuste teórico")
    
    # Exibe parâmetros com incertezas
    perr = np.sqrt(np.diag(pcov))
    print(f"Parâmetros ajustados:")
    print(f"C = {popt[0]:.3e} ± {perr[0]:.1e}")
    print(f"s = {popt[1]:.3f} ± {perr[1]:.3f}")

except Exception as e:
    print(f"Erro no ajuste: {e}")

plt.legend()
plt.tight_layout()
plt.savefig("densidade_foton_ajuste.pdf")
plt.show()