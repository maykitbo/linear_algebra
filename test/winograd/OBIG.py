import pandas as pd
from lmfit import Model
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

data = pd.read_csv('test5_from32.txt', sep=r'\s+', header=None,
                   names=['N', 'W', 'WP', 'S', 'SP', 'CBLAS', 'CLASSICP'])


def case_1(alg_data, alg_name):
    def model(N, a, b, c):
        return a * np.power(N, b) + c

    initial_params, _ = curve_fit(model, alg_data['N'], alg_data[alg_name])
    a, b, c = initial_params
    model = Model(model)
    params = model.make_params(a=a, b=b, c=c)
    return model.fit(alg_data[alg_name], params, N=alg_data['N'])

def case_2(alg_data, alg_name):
    def model(N, a, b, c):
        return a * np.power(N, b) + c * N

    model = Model(model)
    params = model.make_params(a=1, b=3, c=0)
    return model.fit(alg_data[alg_name], params, N=alg_data['N'])

def case_3(alg_data, alg_name):
    def model(N, a, b):
        return a * np.power(N, b)

    model = Model(model)
    params = model.make_params(a=1, b=3)
    return model.fit(alg_data[alg_name], params, N=alg_data['N'])


def one_alg(ax, alg_data, alg_name):
    result = case_1(alg_data, alg_name)
    
    ax.scatter(alg_data['N'], alg_data[alg_name], label='Experimental Data')
    ax.plot(alg_data['N'], result.best_fit, label='Best Fit', color='red')
    ax.set_xlabel('N')
    ax.set_ylabel('Time (nanoseconds)')
    ax.set_title(alg_name)
    ax.legend()
    
    textstr = '\n'.join([f'{param_name}={result.params[param_name].value:.4g}  +/-' \
                         f'{result.params[param_name].stderr:.4g}' \
                         for i, param_name in enumerate(result.params)])
    
    props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
    
    ax.text(0.05, 0.78, textstr, transform=ax.transAxes, fontsize=10,
            verticalalignment='top', bbox=props)

fig, axs = plt.subplots(2, 3, figsize=(15, 10))

algorithms = ['W', 'WP', 'S', 'SP', 'CBLAS', 'CLASSICP']

for i, alg_name in enumerate(algorithms):
    row = i // 3
    col = i % 3
    one_alg(axs[row, col], data, alg_name)

plt.tight_layout()
plt.show()

# def one_alg(alg_data, alg_name):
#     model = Model(model_func)

#     params = model.make_params(a=1, b=3, c=0)

#     result = model.fit(alg_data[alg_name], params, N=alg_data['N'])

#     print(alg_name, ':\n', result.fit_report())

#     # Plot the experimental data and the best-fit curve
#     plt.scatter(alg_data['N'], alg_data[alg_name], label='Experimental Data')
#     plt.plot(alg_data['N'], result.best_fit, label='Best Fit', color='red')
#     plt.xlabel('N')
#     plt.ylabel('Time (nanoseconds)')
#     plt.legend()
#     plt.show()

# one_alg(data, 'W')
# one_alg(data, 'WP')
# one_alg(data, 'S')
# one_alg(data, 'SP')
# one_alg(data, 'CBLAS')
# one_alg(data, 'CLASSICP')
