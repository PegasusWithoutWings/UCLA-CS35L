from random import choices

def encrypt(word):
  result_builder = []
  for char in word:
    encrypted_char = chr(ord(char) ^ 42)
    result_builder.append(encrypted_char)
  result = ''.join(result_builder)
  return result

words = [word.strip() for word in open("/usr/share/dict/words", 'r').readlines()]
output = open("rand_words.txt", 'w')

for word in choices(words, k = 5000000):
  output.write(encrypt(word) + ' ')