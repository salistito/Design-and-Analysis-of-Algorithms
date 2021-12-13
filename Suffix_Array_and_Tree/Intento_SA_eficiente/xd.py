s = "banana"
dic = {}
suffix_array = []
sub = ""
for i in range(len(s)-1, -1, -1):
    sub = s[i] + sub
    dic[sub] = i

print(dic)
suffix_array = [value for key,value in dic.items()]

for i in range(len(s)):
    print(suffix_array[i])

