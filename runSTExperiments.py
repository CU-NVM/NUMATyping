#!/usr/bin/python

import shutil
import subprocess
import sys
import argparse
from pathlib import Path
#threads we want to run for = t:4:8:12:16:20:24:28:32:36:40
def copy_folder(src_folder, dest_folder):
    try:
        # Copy the folder from source to destination
        shutil.copytree(src_folder, dest_folder, dirs_exist_ok=True)
        print(f"Folder copied from {src_folder} to {dest_folder}")
    except Exception as e:
        print(f"Failed to copy folder: {e}")
        sys.exit(1)
        
        
def clear_file(file_path):
    with open(file_path, 'w') as file:
        # Opening in 'w' mode clears the file contents
        pass
        
def run_command(command):
    # Start the command with stdout and stderr redirected to subprocess.PIPE
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, shell=True)

    # Read and print each line from stdout as it becomes available
    while True:
        output = process.stdout.readline()
        if output == "" and process.poll() is not None:
            break
        if output:
            print(output.strip())  # Print output line by line

    # Wait for the process to complete
    process.wait()
    
def run_command_and_redirect_output(command, output_file):
    try:
        # Run the command as a shell process
        with open(output_file, 'w') as file:
            result = subprocess.run(command, shell=True, check=True, text=True, stdout=file)
            print("Command output:", result.stdout)
    except subprocess.CalledProcessError as e:
        print(f"Command failed with error: {e.stderr}")
        sys.exit(1)
        
def create_file(directory, filename):
    # Ensure the directory exists
    Path(directory).mkdir(parents=True, exist_ok=True)
    
    # Create the file within the specified directory
    file_path = Path(directory) / filename
    file_path.touch(exist_ok=True)  # Creates the file if it doesn't exist

    print(f"File created: {file_path}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Copy folder and run a command with an additional argument.")
    parser.add_argument("exprName", type=str, help="experimentName to be run on the src to src tool")
    parser.add_argument('--DS', type=str, required=True, help="Specify the data structure name")
    parser.add_argument('--UMF', action='store_true', help="Enable verbose mode")
    parser.add_argument('--verbose', action='store_true', help="Enable verbose mode")
    
    args = parser.parse_args()

    # Example source and destination directories
    experiment_folder= f"./{args.exprName}"
    clang_tool_input= "./numa-clang-tool/input/"
    clang_tool_intermediate_output = f"./numa-clang-tool/output/{args.exprName}"
    clang_tool_output= f"./numa-clang-tool/output2/{args.exprName}"
    experiment_output= f"./Output/{args.exprName}/"
    
    run_command("echo Stack Results >> ./Result/stack.csv")
    run_command("echo Queue Results >> ./Result/queue.csv")
    run_command("echo LL Results >> ./Result/ll.csv")
    run_command("echo BST Results >> ./Result/bst.csv")

    
    command = f"cp -rf ./{args.exprName} {clang_tool_input}"
    run_command(command)

    # Command to be run after copying (you can modify this as needed)
    command = f"cd numa-clang-tool && sudo ./run.sh {args.exprName}"  # Example command
    print(command)
    # Run the command
    run_command(command)
    
    copy_folder(experiment_folder, clang_tool_input)
    copy_folder(experiment_folder, clang_tool_intermediate_output)
    copy_folder(experiment_folder, clang_tool_output)
    
    copy_folder(clang_tool_output, experiment_output)
        
    run_command(f"chmod -R 777 ./Output/{args.exprName}/")
    
    command = f"cd ./Output/{args.exprName}/ && sudo  make clean"
    
    run_command(command)
    StackCommand = f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:stack --meta th_config:numa:regular --meta DS_config:numa:regular  >> ../../Result/STResult/stack.csv"
    QueueCommand = f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:queue --meta th_config:numa:regular --meta DS_config:numa:regular >> ../../Result/STResult/queue.csv"
    LLCommand = f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:ll --meta th_config:numa:regular --meta DS_config:numa:regular >> ../../Result/STResult/ll.csv"
    BSTCommand = f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:bst --meta th_config:numa:regular --meta DS_config:numa:regular  --meta k:1600000 >> ../../Result/STResult/bst.csv"
 
    if(args.UMF):
        run_command("echo Stack Results (UMF) >> ./Result/stack.csv")
        run_command("echo Queue Results (UMF) >> ./Result/queue.csv")
        run_command("echo LL Results (UMF)>> ./Result/ll.csv")
        run_command("echo BST Results (UMF)>> ./Result/bst.csv")

        command = f"cd ./Output/{args.exprName}/ && make UMF=1"
    else:
        command = f"cd ./Output/{args.exprName}/ && make "
    
    run_command(command)
    if(args.verbose):
        if(args.DS == "bst"):
            run_command("echo Date, Time, DS_name, num_DS, num_threads, thread_config, DS_config, duration, crossover,keyspace, TotalOps")
        else:
            run_command("echo Date, Time, DS_name, num_DS, num_threads, thread_config, DS_config, duration, crossover, TotalOps")
    else:
        if(args.DS == "bst"):
            run_command("echo Date, Time, DS_name, num_DS, num_threads, thread_config, DS_config, duration, crossover,keyspace, TotalOps >> ./Result/STResult/bst.csv")
        else:
            run_command("echo Date, Time, DS_name, num_DS, num_threads, thread_config, DS_config, duration, crossover, TotalOps >> ./Result/STResult/stack.csv")
           
        
        
    
    if (args.DS == "stack"):
        if (args.verbose):
            print("command is ", StackCommand)
            run_command(StackCommand)
        else:
            StackCommand = f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:stack --meta th_config:numa:regular --meta DS_config:numa:regular  >> ../../Result/STResult/stack.csv"
            print("command is ", StackCommand)
            run_command(StackCommand)
    elif (args.DS == "queue"):
        if (args.verbose):
            QueueCommand = f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:queue --meta th_config:numa:regular --meta DS_config:numa:regular  "
            print("command is ", QueueCommand)
            run_command(QueueCommand)
        else:
            QueueCommand = f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:queue --meta th_config:numa:regular --meta DS_config:numa:regular  >> ../../Result/STResult/queue.csv"
            print("command is ", QueueCommand)
            run_command(QueueCommand)
    elif (args.DS == "ll"):
        if (args.verbose):
            LLCommand = f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:ll --meta th_config:numa:regular --meta DS_config:numa:regular  "
            print("command is ", LLCommand)
            run_command(LLCommand)
        else:
            LLCommand = f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:ll --meta th_config:numa:regular --meta DS_config:numa:regular >> ../../Result/STResult/ll.csv"
            print("command is ", LLCommand)
            run_command(LLCommand)
    elif (args.DS == "bst"):
        if (args.verbose):
            BSTCommand =f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:bst --meta th_config:numa:regular --meta DS_config:numa:regular --meta k:160000 "
            print("command is ", BSTCommand)
            run_command(BSTCommand)
        else:
            BSTCommand = f"cd ./Output/{args.exprName}/ && python3 meta.py ./Examples/bin/DSExample --meta n:128:1024 --meta t:1 --meta D:20 --meta DS_name:bst --meta th_config:numa:regular --meta DS_config:numa:regular  --meta k:1600000 >> ../../Result/STResult/bst.csv"
            print("command is ", BSTCommand)
            run_command(BSTCommand)
    
    
    if(args.DS == "all"):
        print("Running Stack : \n", StackCommand ,"\n")
        run_command(StackCommand)
        print("Running Queue : \n", QueueCommand ,"\n")
        run_command(QueueCommand)
        print("Running LL : \n", LLCommand ,"\n")
        run_command(LLCommand)
        print("Running BST : \n", BSTCommand ,"\n")
        run_command(BSTCommand)

    # run_command_and_redirect_output(command, f"./Result/stack.csv")
    
    
    