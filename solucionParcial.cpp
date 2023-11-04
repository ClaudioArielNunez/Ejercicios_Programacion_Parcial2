#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;
#include "parcial2.h"
/*
Generar un archivo con la cantidad de ventas que cada restaurante de la categoría 2 tuvo
este año.
Cada registro debe tener el siguiente formato:
Código de restaurante, nombre de restaurante, y cantidad de ventas
Listar el archivo generado
*/

class VentasCateg{
private:
    int codigoRestaurante;
    char nombre[30];
    int cantVtas;
    bool estado;

public:
    VentasCateg(){}
    VentasCateg(int cod , const char *name ,int c ){
        codigoRestaurante = cod;
        strcpy(nombre, name);
        cantVtas = c;
        estado= true;
    }
    void setCodigoRes(int cod){ codigoRestaurante=cod;}
    void setNombreRest(const char *n){ strcpy(nombre, n);}
    void setCantVentas(int cant){ cantVtas = cant;}
    void setEstado(bool es){ estado = es;}

    int getCodigoRestaurante(){return codigoRestaurante;}

    void mostrar(){
        cout<<"CODIGO: "<<codigoRestaurante<<endl;
        cout<<"NOMBRE: "<<nombre<<endl;
        cout<<"VENTAS: "<<cantVtas<<endl;
        cout<<endl;
    }
    //sobrecarga
    bool operator == (VentasCateg obj){
        if(codigoRestaurante == obj.getCodigoRestaurante()) return true;
        else return false;
    }
};

class ArchivoVentasCat{
private:
    char nombre[30];
public:
    ArchivoVentasCat(const char *n){
        strcpy(nombre, n);
    }
    VentasCateg leerRegistro(int pos){
        VentasCateg reg;
        FILE *p;
        p=fopen(nombre, "rb");
        if(p==NULL) return reg;
        fseek(p, sizeof(VentasCateg)*pos,0);
        fread(&reg, sizeof reg,1, p);
        fclose(p);
        return reg;
    }
    int contarRegistros(){
        FILE *p;
        p=fopen(nombre, "rb");
        if(p==NULL) return -1;
        fseek(p, 0,2);
        int tam=ftell(p);
        fclose(p);
        return tam/sizeof(VentasCateg);
    }
     bool grabarArchivo(VentasCateg reg){
        FILE * pArchivo;
        pArchivo = fopen(nombre, "ab");
        if(pArchivo == NULL) return false;

        bool escribio = fwrite(&reg, sizeof reg, 1, pArchivo);
        fclose(pArchivo);
        return escribio;
     }

};

    void leerArchivoNuevo(){
        VentasCateg reg;
        ArchivoVentasCat archi("ventasPorCategoria.dat");
        int cant = archi.contarRegistros();
        for(int i=0; i<cant; i++){
            reg = archi.leerRegistro(i);
            reg.mostrar();
        }
    }

    void punto1(){
        VentasCateg regVC;
        ArchivoVentasCat archiVC("ventasPorCategoria.dat");

        Restaurante regRes;
        ArchivoRestaurantes archiRes("restaurantes.dat");
        int cantREs = archiRes.contarRegistros();

        Venta regV;
        ArchivoVentas archiV("ventas.dat");
        int cantVtas = archiV.contarRegistros();

        bool grabo = false;
        int cont;
        for(int j=0; j<cantREs; j++){
            regRes = archiRes.leerRegistro(j);
            cont = 0;
            if(regRes.getCategoria() == 2){
                for(int i=0; i<cantVtas; i++){
                    regV = archiV.leerRegistro(i);
                    if(regV.getCodigoRestaurante() == regRes.getCodigoRestaurante() && regV.getFechaReserva().getAnio() == 2023){
                        cont++;
                    }
                }
                ///aca seteo y grabo mi archivo
                regVC.setCodigoRes(regRes.getCodigoRestaurante());
                regVC.setNombreRest(regRes.getNombre());
                regVC.setCantVentas(cont);

                grabo = archiVC.grabarArchivo(regVC);

            }

        }
        if(grabo) cout<<"REGISTROS GRABADOS CON EXITO"<<endl;
        else cout<<"NO SE ENCONTRARON REGISTROS"<<endl;

        leerArchivoNuevo();
    }

//Informar la provincia con menos cantidad de restaurantes de la categoría 1.

    void punto2(){
        int vecProvincias[24] = {0};

        Restaurante reg;
        ArchivoRestaurantes archi("restaurantes.dat");
        int cant = archi.contarRegistros();

        for(int i=0; i<cant; i++){
            reg = archi.leerRegistro(i);
            if(reg.getCategoria() == 1){
                vecProvincias[reg.getProvincia()-1]++;
            }
        }
        int cantMenor=vecProvincias[0];
        //int numProv =1;
        for(int i=0; i<24; i++){
            if(vecProvincias[i] < cantMenor ){
                cantMenor = vecProvincias[i];
                //numProv = i+1;
            }
        }
        cout<<"LA/S PROVINCIA CON MENOR CANTIDAD DE RESTOS (CATEGORIA 1) ES/SON:"<<endl;

        for(int i=0; i<24; i++){
            if(vecProvincias[i] == cantMenor){

                cout<<i+1<<endl;
            }
        }
    }

//Agregar una sobrecarga para el operador == de la clase nueva del punto 1, que reciba
//como parámetro un objeto de la misma clase
/*
    bool operator == (VentasCateg obj){
            if(codigoRestaurante == obj.getCodigoRestaurante()) return true;
            else return false;
        }
*/
///Crear un vector dinámico para copiar y mostrar el archivo creado en el punto1
    void cargarArchivoEnVector(VentasCateg *v){
        VentasCateg reg;
        ArchivoVentasCat archi("ventasPorCategoria.dat");
        int cant = archi.contarRegistros();
        for(int i=0; i<cant; i++){
            reg = archi.leerRegistro(i);
            v[i] = reg;
        }
    }
    void mostrarVector(VentasCateg *v, int tam){
        for(int i=0; i<tam; i++){
            v[i].mostrar();
        }
    }

    void punto4(){
        VentasCateg reg;
        ArchivoVentasCat archi("ventasPorCategoria.dat");
        int cant = archi.contarRegistros();
        if(cant == 0){
            cout<<"NO HAY REGISTROS"<<endl;
            return;
        }
        VentasCateg *pVector;
        pVector = new VentasCateg[cant];
        if(pVector == NULL){
            cout<<"ERROR DE ASIGNACION DE MEMORIA"<<endl;
            return;
        }
        cargarArchivoEnVector(pVector);
        mostrarVector(pVector, cant);

        delete [] pVector;
    }


int main(){

    //punto1();
    //punto2();
    //punto3
///   COMPROBACION DE SOBRECARGA
//    VentasCateg reg(2,"FIGARO",10);
//    VentasCateg reg2(2, "LA FONDA DEL GALLEGO",50);
//    if(reg == reg2) cout<<"ES EL MISMO LUGAR"<<endl;
//    else cout<<"SON LUGARES DISTINTOS"<<endl;
    //punto4();

    return 0;
}
