#include "funciones4-5.h"

#define MASK_VALOR_CANAL_EVENTO 0x7f
#define MASK_EVENTO 0x70
#define MASK_BIT_ALTO 0x80
#define MASK_CANAL 0xf 
#define MASK_BIT_ALTO_APAGAR 0x7f

#define SHIFT_F 2
#define SHIFT_E 4

#define MASK_BIT_ALTO 0x80
#define METAEVENTO_FIN_DE_PISTA 0x2F
#define EVENTO_MAX_LONG 10

const char *formato_cadena[] = {
	[PISTA_UNICA] = "Pista Unica",
	[MULTIPISTAS_SINCRONICAS] = "Multipistas Sincronicas",
	[MULTIPISTAS_ASINCRONICAS] = "Multipistas Asincronicas"
};

const char *evento_cadena[] = {
	[NOTA_APAGADA] = "Nota Apagada",
	[NOTA_ENCENDIDA] = "Nota Encendida",
	[NOTA_DURANTE] = "Nota Durante",
	[CAMBIO_DE_CONTROL] = "Cambio de Control",
	[CAMBIO_DE_PROGRAMA] = "Cambio de Programa",
	[VARIAR_CANAL] = "Variar Canal",
	[CAMBIO_DE_PITCH] = "Cambio de Pitch",
	[METAEVENTO] = "Metaevento"
};

const int longitud_evento[] ={
	[NOTA_APAGADA] = 2,
	[NOTA_ENCENDIDA] = 2,
	[NOTA_DURANTE] = 2,
	[CAMBIO_DE_CONTROL] = 2,
	[CAMBIO_DE_PROGRAMA] = 1,
	[VARIAR_CANAL] = 1,
	[CAMBIO_DE_PITCH] = 2,
	[METAEVENTO] = 2
};

const char *nota_cadena[] = {
	[DO] = "Do",
	[DOS] = "Do#",
	[RE] = "Re",
	[RES] = "Re#",
	[MI] = "Mi",
	[FA] = "Fa",
	[FAS] = "Fa#",
	[SOL] = "Sol",
	[SOLS] = "Sol#",
	[LA] = "La",
	[LAS] = "La#",
	[SI] = "Si"
};
bool decodificar_formato(uint16_t valor, formato_t *formato){
	if(valor > 2)
		return false;
	
	*formato = valor;
	return true;
}

bool decodificar_evento(uint8_t valor, evento_t *evento, char *canal, int *longitud){
	if((valor & MASK_BIT_ALTO) != MASK_BIT_ALTO)
		return false;

	valor &= MASK_BIT_ALTO_APAGAR; 
	*evento = (valor & MASK_EVENTO) >> SHIFT_E;
	*canal = valor & MASK_CANAL;
	*longitud = longitud_evento[*evento];
	return true;
}

bool decodificar_nota(uint8_t valor, nota_t *nota, signed char *octava){
	if((valor & MASK_BIT_ALTO) != 0)
		return false;

	*nota = valor % 12;
	*octava = (valor / 12) - 1;
	return true;
}

const char *codificar_formato(formato_t formato){
	return formato_cadena[formato];
}

const char *codificar_evento(evento_t evento){
	return evento_cadena[evento];
}

const char *codificar_nota(nota_t nota){
	return nota_cadena[nota];
}

uint8_t leer_uint8_t(FILE *f){
	uint8_t v;
	fread(&v, sizeof(uint8_t), 1, f);
	return v;
}

uint16_t leer_uint16_big_endian(FILE *f){
	uint8_t v[2];
	for(size_t i = 0; i < 2; i++)
		fread(&v[i], sizeof(uint8_t), 1, f);
	return v[0] << 8 | v[1];
}

uint32_t leer_uint32_big_endian(FILE *f){
	uint8_t v[4];
	for(size_t i = 0; i < 4; i++)
		fread(&v[i], sizeof(uint8_t), 1, f);
	return v[0] << 24 | v[1] << 16 | v[2] << 8 | v[3];
}

bool leer_encabezado(FILE *f, formato_t *formato, uint16_t *numero_pistas, uint16_t *pulsos_negra){
 	if(leer_uint32_big_endian(f) != 0x4d546864)
 		return false;
 	else if(leer_uint32_big_endian(f) != 6)
 		return false;
 	*formato = leer_uint16_big_endian(f);
 	*numero_pistas = leer_uint16_big_endian(f);
 	*pulsos_negra = leer_uint16_big_endian(f);
 	return true;
}

bool leer_pista(FILE *f, uint32_t *tamagno){
	if(leer_uint32_big_endian(f) != 0x4d54726b)
		return false;
	*tamagno = leer_uint32_big_endian(f);
	return true;
}	

bool leer_tiempo(FILE *f, uint32_t *tiempo){
	uint8_t p = leer_uint8_t(f);
	if((p & MASK_BIT_ALTO) == 0){
		*tiempo = p;
		return true;
	}
	uint8_t r = leer_uint8_t(f);
	if((r & MASK_BIT_ALTO) == 0){
		*tiempo = ((p & 0x7f) << 7) | r;
		return true;
	}
	uint8_t q = leer_uint8_t(f);
	if((q & MASK_BIT_ALTO) == 0){
		*tiempo = ((p & 0x7f) << 14) | ((r & 0x7f) << 7) | q;
		return true;
	}
	uint8_t c = leer_uint8_t(f);
	if((c & MASK_BIT_ALTO) == 0){
		*tiempo = ((p & 0x7f) << 21) | ((r & 0x7f) << 14) | ((q	 & 0x7f) << 7) | c;
		return true;
	}
	return false;
}

bool leer_evento(FILE *f, evento_t *evento, char *canal, int *longitud, uint8_t mensaje[]){
	uint8_t valor = leer_uint8_t(f);
	if(decodificar_evento(valor, evento, canal, longitud) == true){
		for(size_t i = 0; i < (size_t)*longitud; i++)
			mensaje[i] = leer_uint8_t(f);
		return true;
	}
	else{
		mensaje[0] = valor;
		for(size_t i = 1; i < (size_t)*longitud; i++)	
			mensaje[i] = leer_uint8_t(f);
		return true;
	}
	return false;
}

void descartar_metaevento(FILE *f, uint8_t tamagno){
	for(size_t i = 0; i < tamagno; i++)
		leer_uint8_t(f);
}
