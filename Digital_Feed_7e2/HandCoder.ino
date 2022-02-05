void H_Coder()
{
   ///////////////
   // Масштаб
   ///////////////
   if (Motor_Z_Pos == Null_Z_Pos + Hand_Z_Pos && Motor_X_Pos == Null_X_Pos + Hand_X_Pos)
   {
      if (Scale == HC_SCALE_10 && flag_Scale_x1 == ON)
      { 
         Hand_Count = 0;
         Hand_Count_New = 0;
         Hand_Count_Old = 0;
         Hand_Z_Pos = 0;
         Hand_X_Pos = 0;

         Null_Z_Pos = Motor_Z_Pos;
         Null_X_Pos = Motor_X_Pos;

         max_OCR3A = HC_START_SPEED_1;
         min_OCR3A = HC_MAX_SPEED_1;
         
         Scale = HC_SCALE_1;
      }
      else if (Scale == HC_SCALE_1 && flag_Scale_x10 == ON)
      {
         Hand_Count = 0;
         Hand_Count_New = 0;
         Hand_Count_Old = 0;
         Hand_Z_Pos = 0;
         Hand_X_Pos = 0;

         Null_Z_Pos = Motor_Z_Pos;
         Null_X_Pos = Motor_X_Pos;

         max_OCR3A = HC_START_SPEED_10;
         min_OCR3A = HC_MAX_SPEED_10;
         
         Scale = HC_SCALE_10;
      }
   }
   
   /////////////////
   // Активная ось 
   /////////////////
   /////////////////
   if (hand_Z == ON)
   {
      Disa_INT_Hcoder();
      //Hand_Count_New = Hand_Count;
      if (HC_Z_DIR == 0) {Hand_Count_New = Hand_Count;}
      else               {Hand_Count_New = Hand_Count - Hand_Count *2;}
      Ena_INT_Hcoder();
      
      if (Hand_Count_New != Hand_Count_Old)
      {
         Hand_Count_Old = Hand_Count_New;

         Hand_Z_Pos = (Hand_Count_New * Scale * MOTOR_Z_STEP_PER_REV * McSTEP_Z / SCREW_Z +McSTEP_Z /2)  & ~(McSTEP_Z - 1);
         Brake_Compens = max_OCR3A - min_OCR3A +1;
         Disable_INT_OCR3B();
         Enable_INT_OCR3A();
      } 


      if (Motor_Z_Pos < Null_Z_Pos + Hand_Z_Pos)
      {
         Motor_Z_Dir = ZCW;
         Motor_Z_CW();
         Limit_Pos_HC = Null_Z_Pos + Hand_Z_Pos - Brake_Compens;
         Limit_Pos = Limit_Pos_Left - Brake_Compens;
      }
      else if (Motor_Z_Pos > Null_Z_Pos + Hand_Z_Pos)
      {
         Motor_Z_Dir = ZCCW;
         Motor_Z_CCW();
         Limit_Pos_HC = Null_Z_Pos + Hand_Z_Pos + Brake_Compens;
         Limit_Pos = Limit_Pos_Right + Brake_Compens;
      }
   }

   //////////////////////////
   else if (hand_X == ON)
   {
      Disa_INT_Hcoder();
      if (HC_X_DIR == 0) {Hand_Count_New = Hand_Count;}
      else               {Hand_Count_New = Hand_Count - Hand_Count *2;}
      Ena_INT_Hcoder();
      
      if (Hand_Count_New != Hand_Count_Old)
      {
         Hand_Count_Old = Hand_Count_New;

         Hand_X_Pos = (Hand_Count_New * Scale * MOTOR_X_STEP_PER_REV * McSTEP_X / SCREW_X +McSTEP_X /2)  & ~(McSTEP_X - 1);
         Brake_Compens = max_OCR3A - min_OCR3A +1;
         Disable_INT_OCR3A();
         Enable_INT_OCR3B();
      } 


      if (Motor_X_Pos < Null_X_Pos + Hand_X_Pos)
      {
         Motor_X_Dir = CW;
         Motor_X_CW();
         Limit_Pos_HC = Null_X_Pos + Hand_X_Pos - Brake_Compens;
      }
      else if (Motor_X_Pos > Null_X_Pos + Hand_X_Pos)
      {
         Motor_X_Dir = CCW;
         Motor_X_CCW();
         Limit_Pos_HC = Null_X_Pos + Hand_X_Pos + Brake_Compens;
      }
   }
      
   /////////////////////////////////////////////////
   else if (hand_Z == OFF && hand_X == OFF)
   {
     //
   }
}
