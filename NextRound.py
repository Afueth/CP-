n,k = map(int, input().split())
scores = list(map(int, input().split()))

pos = scores[k-1]
count =0

for s in scores:
    if s>=pos and s>0:
        count +=1
        
print(count)