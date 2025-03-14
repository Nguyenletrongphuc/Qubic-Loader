from cipher.caesar import ALPHABET

class CaesarCipher:
    def __init__(self):
        self.alphabet = ALPHABET

    def encrypt_text(self,text: str, key:int) -> str:
        alphabet_len = len(self.alphabet)
        text = text.upper()
        encrypted_text = []
        for letter in text:
            letter_index = self.alphabet.index(letter)
            output_index = (letter_index + key)%alphabet_len
            output_letter = self.alphabet[output_index]
            encrypted_text.append(output_letter)
        return "".join(encrypted_text)
    def dencrypt_text(self,text: str,key:int) -> str:
        alphabet_len = len(self.alphabet)
        text = text.upper()
        dencrypted_text=[]
        for letter in text:
            letter_index = self.alphabet.index(letter)
            output_index = (letter_index - key)%alphabet_len
            output_letter = self.alphabet[output_index]
            dencrypted_text.append(output_letter)
        return "".join(dencrypted_text)