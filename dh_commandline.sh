openssl genpkey -genparam -algorithm DH -out dhp.pem 
openssl genpkey -paramfile dhp.pem -out dhkey1.pem
openssl pkey -in dhkey1.pem -text -noou
openssl genpkey -paramfile dhp.pem -out dhkey2.pem
openssl pkey -in dhkey2.pem -text -noout

openssl pkey -in dhkey1.pem -pubout -out dhpub1.pem
openssl pkey -pubin -in dhpub1.pem -text 

openssl pkey -in dhkey2.pem -pubout -out dhpub2.pem
openssl pkey -pubin -in dhpub2.pem -text 

openssl pkeyutl -derive -inkey dhkey1.pem -peerkey dhpub2.pem -out secret1.bin
openssl pkeyutl -derive -inkey dhkey2.pem -peerkey dhpub1.pem -out secret2.bin