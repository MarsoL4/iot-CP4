import pandas as pd
from pycaret.classification import *
import m2cgen as m2c

# Carregue o CSV
df = pd.read_csv("sensor_dados.csv")
features = ["mean","std","rms","ptp"]
target = "label"

# PyCaret setup
s = setup(data=df[features+[target]], target=target,
          session_id=42, normalize=True, fold=5, verbose=False)

# 1) Decision Tree
dt = create_model("dt")
dt_final = finalize_model(dt)
open("model_dt.c", "w").write(m2c.export_to_c(dt_final))

# 2) SVM
svm = create_model("svm")
svm_final = finalize_model(svm)
open("model_svm.c", "w").write(m2c.export_to_c(svm_final))

# Mostre as métricas
print(pull())  # Mostra a tabela com Accuracy, F1, etc.