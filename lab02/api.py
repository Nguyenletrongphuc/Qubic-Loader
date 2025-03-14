from cipher.vigenere import VigenereCipher
from flask import Flask, request, jsonify
from cipher.caesar import CaesarCipher
from cipher.railfence import Railfence
from cipher.playfail import PlayfairCipher

vigenere_cipher = VigenereCipher()
app = Flask(__name__)
caesar_cipher = CaesarCipher()
railfence_cipher = Railfence()
PlayfairCipher = PlayfairCipher()

@app.route("/api/playfair/creatematrix", methods=["POST"])
def playfair_creatematrix():
    data = request.json
    key=data['key']
    playfair_matrix = PlayfairCipher.create_playfail_matrix(key)
    return jsonify({'playfair_matrix': playfair_matrix})

@app.route("/api/playfair/encrypt", methods=["POST"])
def playfair_encrypt():
    data = request.json
    plain_text = data['plain_text']
    key=data['key']
    playfail_matrix = PlayfairCipher.create_playfail_matrix(key)
    encrypted_text = PlayfairCipher.playfair_encrypt(plain_text, playfail_matrix)
    return jsonify({'encrypted_text': encrypted_text})

@app.route("/api/playfair/decrypt", methods=["POST"])
def playfair_decrypt():
    data = request.json
    cipher_text = data['cipher_text']
    key = data['key']
    playfail_matrix = PlayfairCipher.create_playfail_matrix(key)
    decrypted_text = PlayfairCipher.playfair_decrypt(cipher_text, playfail_matrix)
    return jsonify({'decrypted_text': decrypted_text})

@app.route("/api/railfence/encrypt", methods=["POST"])
def encrypt():
    data=request.json
    plain_text = data['plain_text']
    key = int(data['key'])
    encrypted_text = railfence_cipher.rail_fence_encrypt(plain_text,key)
    return jsonify({'encrypted_text': encrypted_text})

@app.route("/api/railfence/decrypt", methods=["POST"])
def decrypt():
    data=request.json
    cipher_text = data['cipher_text']
    key = int(data['key'])
    decrypted_text = railfence_cipher.rail_fence_decrypt(cipher_text,key)
    return jsonify({'decrypted_text': decrypted_text})
    

@app.route("/api/vigenere/encrypt", methods=["POST"])
def vigenere_encrypt():
    data=request.json
    plain_text = data['plain_text']
    key = data['key']
    encrypted_text = vigenere_cipher.vigenere_encrypt(plain_text,key)
    return jsonify({'encrypted_text': encrypted_text})

@app.route("/api/vigenere/decrypt", methods=["POST"])
def vigenere_decrypt():
    data=request.json
    cipher_text = data['cipher_text']
    key = data['key']
    decrypted_text = vigenere_cipher.vigenere_decrypt(cipher_text,key)
    return jsonify({'decrypted_text': decrypted_text})




@app.route("/api/caesar/encrypt", methods=["POST"])
def caesar_encrypt():
    data=request.json
    plain_text = data['plain_text']
    key = int(data['key'])
    encrypted_text = caesar_cipher.encrypt_text(plain_text,key)
    return jsonify({'encrypted_message': encrypted_text})

@app.route("/api/caesar/decrypt", methods=["POST"])
def caesar_decrypt():
    data=request.json
    cipher_text = data['cipher_text']
    key = int(data['key'])
    decrypted_text = caesar_cipher.dencrypt_text(cipher_text,key)
    return jsonify({'dencrypted_message': decrypted_text})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug = True)

