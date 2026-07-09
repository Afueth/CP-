n=int(input())
words = []
for i in range(n):
    words.append(input())
for word in words:
    if len(word)>10:
       print(word[0] + str(len(word) - 2) + word[-1])
    else: 
         print(word)
         
         
         
# Efficent Method      
# n = int(input())

# for _ in range(n):
#     word = input()
#     if len(word) > 10:
#         print(word[0] + str(len(word) - 2) + word[-1])
#     else:
#         print(word)