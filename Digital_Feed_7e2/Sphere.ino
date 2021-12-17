void Sphere_Ext(bool & a_flag, bool & b_flag)
{
   int Pass_Total = Sph_R_mm * 2 / Cutting_Width;

   if (Motor_X_Pos == Limit_Pos_Rear && Motor_Z_Pos == Limit_Pos_Right && Step_X_flag == OFF)
   {  
      if (cycle_flag == false && Pass_Nr <= Pass_Total +1)
      {
         cycle_flag = true;
         a_flag = false;
         b_flag = false;

         long Infeed_Value;
         {
            if (Pass_Nr <= Pass_Total/2) Infeed_Value = (long)((float)MOTOR_Z_STEP_PER_REV * (Cutting_Width * Pass_Nr) / SCREW_Z + 0.5) *McSTEP_Z;
            else                         Infeed_Value = (long)((float)MOTOR_Z_STEP_PER_REV * ((Cutting_Width * (Pass_Nr-1) + Cutter_Width)) / SCREW_Z +0.5) *McSTEP_Z;
         }
         
         Limit_Pos_Left = (Null_Z_Pos + Infeed_Value);
         Limit_Left_LED_On();
         BeepBeep();
         Feed_Left(a_flag, b_flag);
      }
   }

   else if (Motor_X_Pos == Limit_Pos_Rear && Motor_Z_Pos == Limit_Pos_Left && Step_Z_flag == OFF)
   {
      if (Pass_Nr <= Pass_Total +1)
      {
         a_flag = false;
         b_flag = false;

         long Infeed_Value;
         if (Pass_Nr > Pass_Total / 2)
         {
            long A = Cutting_Width * (Pass_Nr - (Pass_Total/2+1));
            float B = sqrt(R_Quad - A*A);
            float E = (Sph_R_mm - B) * MOTOR_X_STEP_PER_REV / SCREW_X;
            Infeed_Value = ((long)E * McSTEP_X);
            if (Infeed_Value > Sph_R - Bar_R)
            {
               Infeed_Value = Sph_R - Bar_R;
            }
         }
         
         else
         {
            long A = Sph_R_mm - Cutting_Width * Pass_Nr;
            float B = sqrt(R_Quad - A*A);
            float E = (Sph_R_mm - B) * MOTOR_X_STEP_PER_REV / SCREW_X;
            Infeed_Value = ((long)E * McSTEP_X);
         }
         
         Limit_Pos_Front = (Null_X_Pos + Infeed_Value);
         Limit_Pos_Right = Limit_Pos_Left;
         Limit_Front_LED_On();
         
         Feed_Front(a_flag, b_flag);
      }
   }

   else if (Motor_X_Pos == Limit_Pos_Front && Motor_Z_Pos == Limit_Pos_Left && Step_Z_flag == OFF)
   {
      cycle_flag = false;
      a_flag = false;
      b_flag = false;
      
      Limit_Pos_Rear = (Null_X_Pos - REBOUND_X);
      Pass_Nr++;
      if (Motor_X_Pos >= (Limit_Pos_Rear + ((MIN_RAPID_MOTION - MAX_RAPID_MOTION) * REPEAt) *2)) {Rapid_Feed_Rear(a_flag, b_flag);}
      else                                                                                       {Feed_Rear(a_flag, b_flag);}
   }

   if (Pass_Nr > Pass_Total +1)
   {
      Limit_Pos_Left = Limit_Pos_Max;
      Limit_Left_LED_Off();
      Limit_Pos_Right = Limit_Pos_Min;
      Limit_Right_LED_Off();
      Limit_Pos_Front = Limit_Pos_Max;
      Limit_Front_LED_Off();
      Limit_Pos_Rear = Limit_Pos_Min;
      Limit_Rear_LED_Off();
   }
}
