# BMI Calculation with Naive, Multiprocessing, and Multithreading Approaches 🌟

Welcome to the BMI Calculation project! This program calculates the average Body Mass Index (BMI) of individuals using three distinct approaches: Naive, Multiprocessing, and Multithreading. It also measures and compares the execution time of each approach. 📊💻

## ✨ Description
This program reads a list of people’s height and weight from a CSV file (`bmi.csv`), calculates the BMI for each individual, and computes the average BMI using three methods:

1. **Naive Approach**: 🧑‍💻 Sequential processing (one after another).
2. **Multiprocessing Approach**: ⚡ Divides the task into multiple processes (4 processes) that run concurrently.
3. **Multithreading Approach**: 🧵 Uses threads to calculate BMI concurrently with proper synchronization.

The program also tracks and prints the execution time for each approach to highlight performance differences. ⏱️

## 🌟 Features
- **Input from CSV File**: 📜 The program reads data (height and weight) from `bmi.csv`.
- **Three Approaches**: 
  - **Naive** (sequential processing)
  - **Multiprocessing** (using 4 processes)
  - **Multithreading** (using 4 threads)
- **Execution Time Measurement**: 🕒 Tracks the time taken for each approach.
- **Error Handling**: ⚠️ Manages errors like missing files, invalid input, and process/thread creation failures.
- **Concurrency**: Utilizes pipes for multiprocessing and mutexes for multithreading to handle concurrent operations.

## 🚀 Getting Started
To run this project, make sure you have a C environment set up with the necessary libraries.


## Input File**: 
   Ensure that `bmi.csv` is in the following format:
   ```
   Name,Height,Weight
   John,175,70
   Jane,160,55
   ...
   ```

 ## Output: 
   The program will print the average BMI and execution time for each approach. 🌟

## 💡 Code Explanation


### Function Overview:

- **BmiCalculate**: 🍎 Calculates the BMI using the formula: `BMI = weight / (height * height)` (height in meters).
- **ReadfromFile**: 📥 Reads the data from `bmi.csv` into an array of structures.
- **Naive**: 🔢 Sequentially processes all individuals and calculates the average BMI.
- **Multiprocessing**: 🚀 Uses 4 child processes to calculate BMI concurrently. The parent process aggregates the results using pipes.
- **Multithreading**: 🧵 Spawns 4 threads to process data concurrently, with a mutex to protect shared resources.

### Main Function:
- Reads the number of individuals from the CSV file.
- Calls the three approaches (Naive, Multiprocessing, and Multithreading) to compute and display the average BMI and execution time.

## ✍️ Author Yara Khattab

📧 Email: yarakhattab16@gmail.com



🔗 GitHub: github.com/yarakhattab

