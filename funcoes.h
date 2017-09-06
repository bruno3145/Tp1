void movimentoTeclas(unsigned char tecla, int* aa, int* ww, int* ss, int* dd){

	if(tecla=='w'){
		*aa=0;
		*ss=0;
		*dd=0;
		*ww=1;	
	}
	if(tecla=='s'){
		*aa=0;
		*ww=0;
		*dd=0;
		*ss=1;
	}
	if(tecla=='d'){
		*aa=0;
		*ss=0;
		*ww=0;
		*dd=1;
		}
	if(tecla=='a'){
		*ww=0;
		*ss=0;
		*dd=0;
		*aa=1;
		}
}


void atravessaParede(int* x, int* y, int* aa, int* ww, int* ss, int* dd){
	if(*x>110){
		*x=-10;
	}
	if(*y>110){
		*y=-10;
	}
	if(*y<-10){
		*y+=110;
	}
	if(*x<-10){
		*x+=110;
	}
	 if(*ww==1){
		*y+=0.7;
	 }
	 if(*ss==1){
		*y-=0.7;
	 }
	 if(*aa==1){
		*x-=0.7;
	 }
	if(*dd==1){
		*x+=0.7;
	}
}
