from subprocess import call
import os
import shutil
import glob


def copy_best(exec_path, target_path, experiment):
    files = glob.glob(os.path.join(exec_path, '*result_best.obj'))
    for file in files:
        filename = experiment + "_" + os.path.basename(file)
        shutil.copy(file, os.path.join(target_path, filename))

def run_experiment(path_to_binary, binary, exec_path, config):
    try:
        retcode = call(os.path.join(path_to_binary, binary) + " " + config + " -batchmode", cwd=exec_path, shell=True)
#        retcode = call(os.path.join(path_to_binary, binary) + " " + config, cwd=exec_path, shell=True)
        print("Child returned" + str(retcode))
    except OSError as e:
        print("Execution failed: "+str(e))

#adapt this
path="C:/procgen/eval/src/Debug"
executable="procrenderer.exe"

experiment_path = "C:/procgen/gpuprocgenetics/run/experiments"
best_path = "C:/procgen/gpuprocgenetics/run/experiments/best"

experiments = [
"tree_3_mickey",
]

dont_run = [
"city_filling_shoe",
"city_filling_shoe_long",
"city_filling_simpler_whale",
"city_filling_simpler_whale_long",
"city_filling_volume",
"city_filling_whale",
"city_sep_shoe",
"city_sep_volume",
"city_sep_whale",
"city_shoe",
"city_volume",
"city_whale",
"eg_target",
"spaceship_volume",
"tree_3_mickey_holes",
"tree_3_v1_avoid",
"tree_3_v1_volume",
"tree_3_v2_avoid",
"tree_3_v2_mickey",
"tree_3_v2_mikey_holes",
"tree_3_v2_volume",
"tree_sketch1",
]

for experiment in experiments:
    run_experiment(path, executable, os.path.join(experiment_path,experiment), "ga.cfg")
    run_experiment(path, executable, os.path.join(experiment_path,experiment), "mh.cfg")
    run_experiment(path, executable, os.path.join(experiment_path,experiment), "smc.cfg")
    run_experiment(path, executable, os.path.join(experiment_path,experiment), "sosmc.cfg")

#print "experiments done"

for experiment in experiments:
    copy_best(os.path.join(experiment_path,experiment), best_path, experiment)

print "copy done"


