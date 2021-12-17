//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ********** Режим "Резьба" ********** //////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Thread_Left(bool & c_flag, bool & d_flag)
{
   if (c_flag == true) return;
   d_flag = false;
   c_flag = true;

   Joy_X_flag = OFF;
   if (Motor_Z_Pos < (Limit_Pos_Left - (THRD_ACCEL * REPEAt )* 2))
   {
      Ks_Divisor = Thread_Info[Thread_Step].Ks_Div_Z;
      if (tmp_Ks_Divisor != Ks_Divisor)
      {
         tmp_Accel = THRD_ACCEL + Ks_Divisor;
         tmp_Ks_Divisor = THRD_ACCEL + Ks_Divisor;
      }
      Brake_Compens = THRD_ACCEL * REPEAt + 1;
   }
   else
   {
      Ks_Divisor = THRD_ACCEL + Thread_Info[0].Ks_Div_Z;
      tmp_Accel = Ks_Divisor;
      tmp_Ks_Divisor = Ks_Divisor;
      Brake_Compens = tmp_Accel - Ks_Divisor + 1;
   }
   
   Km_Divisor = Thread_Info[Thread_Step].Km_Div_Z;
   Ks_Count = 0;
   Km_Count = 0;
   Limit_Pos = Limit_Pos_Left - Brake_Compens;

   Motor_Z_Dir = CW;
   Motor_Z_CW();
   if (Read_Z_Ena_State == false) Motor_Z_Enable();
   Joy_Z_flag = ON;
}
  

void Thread_Right(bool & c_flag, bool & d_flag)
{
   if (d_flag == true) return;
   c_flag = false;
   d_flag = true;

   Joy_X_flag = OFF;
   if (Motor_Z_Pos > Limit_Pos_Right + THRD_ACCEL * REPEAt * 2 || Motor_Z_Pos <= Limit_Pos_Right)
   {
      Ks_Divisor = Thread_Info[Thread_Step].Ks_Div_Z;
      if (tmp_Ks_Divisor != Ks_Divisor)
      {
         tmp_Accel = THRD_ACCEL + Ks_Divisor;
         tmp_Ks_Divisor = THRD_ACCEL + Ks_Divisor;
      }  
      Brake_Compens = THRD_ACCEL * REPEAt + 1;
   }
   else
   {
      Ks_Divisor = THRD_ACCEL + Thread_Info[0].Ks_Div_Z;
      tmp_Accel = Ks_Divisor;
      tmp_Ks_Divisor = Ks_Divisor;
      Brake_Compens = tmp_Accel - Ks_Divisor + 1;
   }

   Km_Divisor = Thread_Info[Thread_Step].Km_Div_Z;
   Ks_Count = 0;
   Km_Count = 0;
   Limit_Pos = Limit_Pos_Right + Brake_Compens;

   Motor_Z_Dir = CCW;
   Motor_Z_CCW();
   if (Read_Z_Ena_State == false) Motor_Z_Enable();
   Joy_Z_flag = ON;
}


void Thread_Front(bool & c_flag, bool & d_flag)
{
   if (c_flag == true) return;
   d_flag = false;
   c_flag = true;

   Joy_Z_flag = OFF;
   if (Motor_X_Pos < (Limit_Pos_Front - (THRD_ACCEL * REPEAt) * 2))
   {
    
      if (Sub_Mode_Thread == Sub_Mode_Thread_Man)
      {
         Ks_Divisor = Thread_Info[Thread_Step].Ks_Div_X;
         Km_Divisor = Thread_Info[Thread_Step].Km_Div_X;
      }
      else
      {
         Ks_Divisor = Thread_Info[Thread_Step].Ks_Div_Z;
         Km_Divisor = Thread_Info[Thread_Step].Km_Div_Z;
         Ks_Divisor = (Ks_Divisor + (float)(Km_Divisor + 5000) /10000) * ((float)McSTEP_Z / McSTEP_X);
         Km_Divisor = 0;
      }

      if (tmp_Ks_Divisor != Ks_Divisor)
      {
         tmp_Accel = THRD_ACCEL + Ks_Divisor;
         tmp_Ks_Divisor = THRD_ACCEL + Ks_Divisor;
      }   
      Brake_Compens = THRD_ACCEL * REPEAt + 1;
   }
   else
   {
      Ks_Divisor = THRD_ACCEL + Thread_Info[0].Ks_Div_X;
      Km_Divisor = 0;
      tmp_Accel = Ks_Divisor;
      tmp_Ks_Divisor = Ks_Divisor;
      Brake_Compens = tmp_Accel - Ks_Divisor + 1;
   }
  
   Ks_Count = 0;
   Km_Count = 0;
   Limit_Pos = Limit_Pos_Front - Brake_Compens;

   Motor_X_Dir = CW;
   Motor_X_CW();
   if (Read_X_Ena_State == false) Motor_X_Enable();
   Joy_X_flag = ON;
}


void Thread_Rear(bool & c_flag, bool & d_flag)
{
   if (d_flag == true) return;
   c_flag = false;
   d_flag = true;

   Joy_Z_flag = OFF;
   if (Motor_X_Pos > (Limit_Pos_Rear + (THRD_ACCEL * REPEAt) * 2))
   {
      if (Sub_Mode_Thread == Sub_Mode_Thread_Man)
      {
         Ks_Divisor = Thread_Info[Thread_Step].Ks_Div_X;
         Km_Divisor = Thread_Info[Thread_Step].Km_Div_X;
      }
      else
      {
         Ks_Divisor = Thread_Info[Thread_Step].Ks_Div_Z;
         Km_Divisor = Thread_Info[Thread_Step].Km_Div_Z;
         Ks_Divisor = (Ks_Divisor + (float)(Km_Divisor + 5000) /10000) * ((float)McSTEP_Z / McSTEP_X);
         Km_Divisor = 0;
      }
      
      if (tmp_Ks_Divisor != Ks_Divisor)
      {
         tmp_Accel = THRD_ACCEL + Ks_Divisor;
         tmp_Ks_Divisor = THRD_ACCEL + Ks_Divisor;
      }     
      Brake_Compens = THRD_ACCEL * REPEAt + 1;
   }
   else
   {
      Ks_Divisor = THRD_ACCEL + Thread_Info[0].Ks_Div_X;
      Km_Divisor = 0;
      tmp_Accel = Ks_Divisor;
      tmp_Ks_Divisor = Ks_Divisor;
      Brake_Compens = tmp_Accel - Ks_Divisor + 1;
   }

   Ks_Count = 0;
   Km_Count = 0;
   Limit_Pos = Limit_Pos_Rear + Brake_Compens;

   Motor_X_Dir = CCW;
   Motor_X_CCW();
   if (Read_X_Ena_State == false) Motor_X_Enable();
   Joy_X_flag = ON;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ********** Режим "Цикл Резьба" ********** /////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Thread_Ext_Left()
{
   if ((Motor_Z_Pos == Limit_Pos_Right && Motor_X_Pos == Limit_Pos_Rear) || (Motor_Z_Pos == Limit_Pos_Right && Motor_X_Pos == Null_X_Pos))
   {  
      Pass_Total = Thread_Info[Thread_Step].Pass;
      if (cycle_flag == false && Pass_Nr <= Pass_Total)
      {
         cycle_flag = true;
         c_flag = false;
         d_flag = false;
         
         float Step_mm = Thread_Info[Thread_Step].Step;
         float Pass_Depth;
         if(Pass_Nr == 1) {Pass_Depth = ((Step_mm * 0.866) - (Step_mm * 0.866 / 6) - (Step_mm * 0.866 / 8)) / sqrt(Pass_Total-1) * sqrt(0.3);}
         else             {Pass_Depth = ((Step_mm * 0.866) - (Step_mm * 0.866 / 6) - (Step_mm * 0.866 / 8)) / sqrt(Pass_Total-1) * sqrt(Pass_Nr-1);}
         long Infeed_Value = long(Pass_Depth / ((float)SCREW_X/100 / MOTOR_X_STEP_PER_REV) + 0.5) * McSTEP_X + McSTEP_X;

         Limit_Pos_Front = (Null_X_Pos + Infeed_Value);
         Limit_Front_LED_On();
         Pass_Nr++;
         BeepBeep();
         Thread_Front(c_flag, d_flag);
      }

      else if ((cycle_flag == false) && (Pass_Nr > Pass_Total && Pass_Nr <= Pass_Total + PASS_FINISH))
      {
         cycle_flag = true;
         c_flag = false;
         d_flag = false;

         Pass_Nr++;
         BeepBeep();
         Thread_Front(c_flag, d_flag);
      }
      
      else if (cycle_flag == false && Pass_Nr > Pass_Total + PASS_FINISH)
      {
         c_flag = false;
         d_flag = false;
         
         Limit_Pos_Front = Null_X_Pos;
         Thread_Front(c_flag, d_flag);
         
         Limit_Front_LED_Off();
         Limit_Rear_LED_Off();
         Limit_Pos_Front = Limit_Pos_Max;
         Limit_Pos_Rear = Limit_Pos_Min;         
      }
   }

   else if (Motor_Z_Pos == Limit_Pos_Right && Motor_X_Pos == Limit_Pos_Front)
   {
      c_flag = false;
      d_flag = false;

      Thread_Left(c_flag, d_flag);
   }

   else if (Motor_Z_Pos == Limit_Pos_Left && Motor_X_Pos == Limit_Pos_Front)
   {
      cycle_flag = false;
      c_flag = false;
      d_flag = false;
      
      Limit_Pos_Rear = (Null_X_Pos - REBOUND_X);
      Limit_Rear_LED_On();
      
      Thread_Rear(c_flag, d_flag);
   }

   else if (Motor_Z_Pos == Limit_Pos_Left && Motor_X_Pos == Limit_Pos_Rear)
   {
      c_flag = false;
      d_flag = false;

      Thread_Right(c_flag, d_flag);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Thread_Ext_Right()  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   if ((Motor_Z_Pos == Limit_Pos_Left && Motor_X_Pos == Limit_Pos_Rear) || (Motor_Z_Pos == Limit_Pos_Left && Motor_X_Pos == Null_X_Pos))
   {  
      Pass_Total = Thread_Info[Thread_Step].Pass;
      if (cycle_flag == false && Pass_Nr <= Pass_Total)
      {
         cycle_flag = true;
         c_flag = false;
         d_flag = false;
         
         float Step_mm = Thread_Info[Thread_Step].Step;
         float Pass_Depth = 0;
         if(Pass_Nr == 1) {Pass_Depth = ((Step_mm * 0.866) - (Step_mm * 0.866 / 6) - (Step_mm * 0.866 / 8)) / sqrt(Pass_Total-1) * sqrt(0.3);}
         else             {Pass_Depth = ((Step_mm * 0.866) - (Step_mm * 0.866 / 6) - (Step_mm * 0.866 / 8)) / sqrt(Pass_Total-1) * sqrt(Pass_Nr-1);}
         long Infeed_Value = long(Pass_Depth / ((float)SCREW_X/100 / MOTOR_X_STEP_PER_REV) + 0.5) * McSTEP_X + McSTEP_X;
         Limit_Pos_Front = (Null_X_Pos + Infeed_Value);
         Limit_Front_LED_On();
         Pass_Nr++;

         BeepBeep();
         Thread_Front(c_flag, d_flag);
      }

      else if ((cycle_flag == false) && (Pass_Nr > Pass_Total && Pass_Nr <= Pass_Total + PASS_FINISH))
      {
         cycle_flag = true;
         c_flag = false;
         d_flag = false;

         Pass_Nr++;
         BeepBeep();
         Thread_Front(c_flag, d_flag);
      }
      
      else if (cycle_flag == false && Pass_Nr > Pass_Total + PASS_FINISH)
      {
         c_flag = false;
         d_flag = false;
         
         Limit_Pos_Front = Null_X_Pos;
         Thread_Front(c_flag, d_flag);

         Limit_Front_LED_Off();
         Limit_Rear_LED_Off();
         Limit_Pos_Front = Limit_Pos_Max;
         Limit_Pos_Rear = Limit_Pos_Min;
      }
   }

   else if (Motor_Z_Pos == Limit_Pos_Left && Motor_X_Pos == Limit_Pos_Front)
   {
      c_flag = false;
      d_flag = false;

      Thread_Right(c_flag, d_flag);
   }

   else if (Motor_Z_Pos == Limit_Pos_Right && Motor_X_Pos == Limit_Pos_Front)
   {
      cycle_flag = false;
      c_flag = false;
      d_flag = false;
      
      Limit_Pos_Rear = (Null_X_Pos - REBOUND_X);
      Limit_Rear_LED_On();

      Thread_Rear(c_flag, d_flag);
   }

   else if (Motor_Z_Pos == Limit_Pos_Right && Motor_X_Pos == Limit_Pos_Rear)
   {
      c_flag = false;
      d_flag = false;

      Thread_Left(c_flag, d_flag);
   }   
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Thread_Int_Left()  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   if ((Motor_Z_Pos == Limit_Pos_Right && Motor_X_Pos == Limit_Pos_Front) || (Motor_Z_Pos == Limit_Pos_Right && Motor_X_Pos == Null_X_Pos))
   {  
      Pass_Total = Thread_Info[Thread_Step].Pass;
      if (cycle_flag == false && Pass_Nr <= Pass_Total)
      {
         cycle_flag = true;
         c_flag = false;
         d_flag = false;
         
         float Step_mm = Thread_Info[Thread_Step].Step;
         float Pass_Depth = 0;
         if(Pass_Nr == 1) {Pass_Depth = ((Step_mm * 0.866) - (Step_mm * 0.866 / 4) - (Step_mm * 0.866 / 18)) / sqrt(Pass_Total-1) * sqrt(0.3);}
         else             {Pass_Depth = ((Step_mm * 0.866) - (Step_mm * 0.866 / 4) - (Step_mm * 0.866 / 18)) / sqrt(Pass_Total-1) * sqrt(Pass_Nr-1);}
         long Infeed_Value = long(Pass_Depth / ((float)SCREW_X/100 / MOTOR_X_STEP_PER_REV) + 0.5) * McSTEP_X + McSTEP_X;
         Limit_Pos_Rear = (Null_X_Pos - Infeed_Value);
         Limit_Rear_LED_On();
         Pass_Nr++;

         BeepBeep();
         Thread_Rear(c_flag, d_flag);
      }

      else if ((cycle_flag == false) && (Pass_Nr > Pass_Total && Pass_Nr <= Pass_Total + PASS_FINISH))
      {
         cycle_flag = true;
         c_flag = false;
         d_flag = false;

         Pass_Nr++;
         BeepBeep();
         Thread_Rear(c_flag, d_flag);
      }
      
      else if (cycle_flag == false && Pass_Nr > Pass_Total + PASS_FINISH)
      {
         c_flag = false;
         d_flag = false;
         
         Limit_Pos_Rear = Null_X_Pos;
         Thread_Rear(c_flag, d_flag);

         Limit_Front_LED_Off();
         Limit_Rear_LED_Off();
         Limit_Pos_Front = Limit_Pos_Max;
         Limit_Pos_Rear = Limit_Pos_Min;
      }
   }

   else if (Motor_Z_Pos == Limit_Pos_Right && Motor_X_Pos == Limit_Pos_Rear)
   {
      c_flag = false;
      d_flag = false;

      Thread_Left(c_flag, d_flag);
   }

   else if (Motor_Z_Pos == Limit_Pos_Left && Motor_X_Pos == Limit_Pos_Rear)
   {
      cycle_flag = false;
      c_flag = false;
      d_flag = false;
      
      Limit_Pos_Front = (Null_X_Pos + REBOUND_X);
      Limit_Front_LED_On();

      Thread_Front(c_flag, d_flag);
   }

   else if (Motor_Z_Pos == Limit_Pos_Left && Motor_X_Pos == Limit_Pos_Front)
   {
      c_flag = false;
      d_flag = false;

      Thread_Right(c_flag, d_flag);
   } 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Thread_Int_Right()  //////////////////////////////////////////////////////////////////////////////////////////////////////////
{
   if ((Motor_Z_Pos == Limit_Pos_Left && Motor_X_Pos == Limit_Pos_Front) || (Motor_Z_Pos == Limit_Pos_Left && Motor_X_Pos == Null_X_Pos))
   {  
      Pass_Total = Thread_Info[Thread_Step].Pass ;
      if (cycle_flag == false && Pass_Nr <= Pass_Total)
      {
         cycle_flag = true;
         c_flag = false;
         d_flag = false;
         
         float Step_mm = Thread_Info[Thread_Step].Step;
         float Pass_Depth = 0;
         if(Pass_Nr == 1) {Pass_Depth = ((Step_mm * 0.866) - (Step_mm * 0.866 / 4) - (Step_mm * 0.866 / 18)) / sqrt(Pass_Total-1) * sqrt(0.3);}
         else             {Pass_Depth = ((Step_mm * 0.866) - (Step_mm * 0.866 / 4) - (Step_mm * 0.866 / 18)) / sqrt(Pass_Total-1) * sqrt(Pass_Nr-1);}
         long Infeed_Value = long(Pass_Depth / ((float)SCREW_X/100 / MOTOR_X_STEP_PER_REV) + 0.5) * McSTEP_X + McSTEP_X;
         Limit_Pos_Rear = (Null_X_Pos - Infeed_Value);
         Limit_Rear_LED_On();
         Pass_Nr++;

         BeepBeep();
         Thread_Rear(c_flag, d_flag);
      }

      else if ((cycle_flag == false) && (Pass_Nr > Pass_Total && Pass_Nr <= Pass_Total + PASS_FINISH))
      {
         cycle_flag = true;
         c_flag = false;
         d_flag = false;

         Pass_Nr++;
         BeepBeep();
         Thread_Rear(c_flag, d_flag);
      }
      
      else if (cycle_flag == false && Pass_Nr > Pass_Total + PASS_FINISH)
      {
         c_flag = false;
         d_flag = false;
         
         Limit_Pos_Rear = Null_X_Pos;
         Thread_Rear(c_flag, d_flag);

         Limit_Front_LED_Off();
         Limit_Rear_LED_Off();
         Limit_Pos_Front = Limit_Pos_Max;
         Limit_Pos_Rear = Limit_Pos_Min;
      }
   }

   else if (Motor_Z_Pos == Limit_Pos_Left && Motor_X_Pos == Limit_Pos_Rear)
   {
      c_flag = false;
      d_flag = false;

      Thread_Right(c_flag, d_flag);
   }

   else if (Motor_Z_Pos == Limit_Pos_Right && Motor_X_Pos == Limit_Pos_Rear)
   {
      cycle_flag = false;
      c_flag = false;
      d_flag = false;
      
      Limit_Pos_Front = (Null_X_Pos + REBOUND_X);
      Limit_Front_LED_On();

      Thread_Front(c_flag, d_flag);
   }

   else if (Motor_Z_Pos == Limit_Pos_Right && Motor_X_Pos == Limit_Pos_Front)
   {
      c_flag = false;
      d_flag = false;

      Thread_Left(c_flag, d_flag);
   } 
}
