//
//  functions_Hacker5_4_to_6.cpp
//  hackPracticeMar21
//
//  Created by Ariana Bruno on 4/23/18.
//  Copyright © 2018 Ariana Bruno. All rights reserved.
//

#include "functions_Hacker5_4_to_6.hpp"

double func_f( double x){
    return exp(100.0*x) - 1;
}

double x_step( double x){
    
    double delta_x = -1.0*(1.0/((func_f(1.0001*x) - func_f(x))/(0.0001*x)))*func_f(x);
    
    return delta_x;
}

double calc_norm( vector<double>* x ){
    double sum = 0.0;
    for( int i = 0; i<x->size(); i++){
        sum += pow((*x)[i],2);
    }
    return sqrt(sum);
}

double func_v( vector<double>* x){
    double x1 = (*x)[0];
    double x2 = (*x)[1];
    
    double term1 = (3.0*pow(x1,2.0)) + x2 - 4.0;
    double term2 = (pow(x1,2.0)) - 3.0*x2 + 2.0;
    
    return (pow(term1,2.0) + pow(term2,2.0));
    
}

void calc_v_gradient( vector<double>* x, vector<double>* v_gradient ){
    double x1 = (*x)[0];
    double x2 = (*x)[1];
    
    double v_gradient_x1 = 40*x1*(pow(x1, 2.0) - 1);
    double v_gradient_x2 = 20*(x2 - 1);
    
    v_gradient->push_back(v_gradient_x1);
    v_gradient->push_back(v_gradient_x2);
    
}

void calc_hessian( vector<double>* x, vector<vector<double>>* H_matrix ){
    double x1 = (*x)[0];
    double x2 = (*x)[1];
    
    double H_mat_Vx1_dx1 = (120*pow(x1,2.0)) - 40;
    double H_mat_Vx1_dx2 = 0.0;
    double H_mat_Vx2_dx1 = 0.0;
    double H_mat_Vx2_dx2 = 20;
    
    vector<double> H_mat_1 = { H_mat_Vx1_dx1, H_mat_Vx1_dx2};
    vector<double> H_mat_2 = { H_mat_Vx2_dx1, H_mat_Vx2_dx2};
    
    H_matrix->push_back(H_mat_1);
    H_matrix->push_back(H_mat_2);
}

void calc_Newton_NL_dx( vector<double>* v_gradient, vector<vector<double>>* H_matrix, vector<double>* dx){
    for( int i =0; i< H_matrix->size(); i++){
        double product_sum = 0;
        for( int j = 0; j < (*v_gradient).size(); j++ ){
            double v = (*v_gradient)[j];
            double value = ((*H_matrix)[i][j] == 0.0 ) ? 0.0 : (v*(-1.0/(*H_matrix)[i][j]));
            product_sum = product_sum + value;
        }
        dx->push_back(product_sum);
    }
}

void vector_matrix_product(vector<vector<double>>* mat, vector<double>* vec, vector<double>* result){
    for(int i = 0; i< mat->size(); i++){
        double product = 0;
        for( int j = 0; j<vec->size(); j++){
            product = product + (*vec)[j]*(*mat)[j][i];
        }
        result->push_back(product);
    }
}

double vector_dot( vector<double>* v1, vector<double>* v2 ){
    if( v1->size() != v2->size()){
        cout << "CANNOT SUM VECTORS OF DIFFERENT DIMENTIONS ( X AND DX ) " << endl;
        return -1;
    }
    double dot = 0;
    for( int i = 0; i < v1->size(); i++ ){
        double product = (*v1)[i]*(*v2)[i];
        dot = dot + product;
    }
    return dot;
}

void new_x(vector<double>* x, vector<double>* dx){
    if( x->size() != dx->size()){
        cout << "CANNOT SUM VECTORS OF DIFFERENT DIMENTIONS ( X AND DX ) " << endl;
        return;
    }
    for( int i = 0; i < x->size(); i++ ){
        (*x)[i] = (*x)[i] + (*dx)[i];
    }
}

double new_v_result(vector<double>* x, vector<double>* dx, vector<double>* v_gradient, vector<vector<double>>* H_matrix){
    double current_v = func_v(x);
    double v_grad_dx = vector_dot(v_gradient, dx);
    vector<double> H_mat_dx;
    vector_matrix_product(H_matrix, dx, &H_mat_dx);
    double dx_H_dx = vector_dot(dx, &H_mat_dx);
    double new_v = current_v + v_grad_dx + (0.5)*dx_H_dx;
    return new_v;
    
}

void scale_vec( vector<double>* x, double scalar, vector<double>* scale_x ){
    for( int i = 0; i<x->size(); i++){
        scale_x->push_back((*x)[i] * scalar);
    }
}

void steepest_decent_step(vector<double>* x, vector<double>* dx, vector<double>* t, double solution){
    vector<double> new_x;
    for( int i = 0; i<x->size(); i++){
        double x_i = (*x)[i];
        double x_incr = 0.0;
        if(x_i == 0.0){
            x_incr = 1e-4;
        }else{
            x_incr = x_i + ((1e-4)*x_i);
        }
        vector<double> temp_x = (*x);
        temp_x[i] = x_incr;
        double V_x = func_v(x);
        double V_x_incr = func_v(&temp_x);
        double V_grad = (x_i != 0.0) ? (V_x_incr-V_x)/((1e-4)*x_i) : (V_x_incr-V_x)/((1e-4));
        
        double min_v = func_v(x);
        double min_t = 1;
        double min_x_incr = (*x)[i];
        double min_dx = 0.0;
        
        for( int j = 0; j<= 20; j++){
            double t = pow(2.0, -j);
            double dx = -1.0*t*V_grad;
            vector<double> x_new = *x;
            x_new[i] = x_new[i] + dx;
            
            if( abs(func_v(&x_new)-solution) < abs(min_v - solution) ){
                min_v = func_v(&x_new);
                min_t = t;
                min_x_incr = x_new[i];
                min_dx = dx;
            }
        }
        new_x.push_back(min_x_incr);
        dx->push_back(min_dx);
        t->push_back(min_t);
    }
    (*x) = new_x;
}
