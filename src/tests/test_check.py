# USE: py3 test_check.py output1.txt output2.txt
import sys
if __name__ == "__main__":
  if (len(sys.argv) > 2):
    f_name_1 = sys.argv[1]
    f_name_2 = sys.argv[2]
    with open(f_name_1, "r") as file_1:
      output_1 = set(file_1.read().splitlines())
    with open(f_name_2, "r") as file_2:
      output_2 = set(file_2.read().splitlines())
    diff = output_1.symmetric_difference(output_2)
    if len(diff) == 0:
      print("Correct")
    else:
      print(diff)