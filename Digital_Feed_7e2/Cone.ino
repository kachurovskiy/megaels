/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ********** Cone Mode ********** //////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Cone_Left(bool & a_flag, bool & b_flag)
{  
   if (b_flag == true) return;
   a_flag = false;
   b_flag = true;
   
   Cs_Divisor = Cone_Info[Cone_Step].Cs_Div;
   Cm_Divisor = Cone_Info[Cone_Step].Cm_Div;

   Start_Speed = ENC_LINE_PER_REV / ((uint32_t)MOTOR_Z_STEP_PER_REV * McSTEP_Z * MIN_FEED / SCREW_Z) / FEED_ACCEL /2;
   if (Motor_Z_Pos < Limit_Pos_Left - Start_Speed * 2)
   {
      Feed_Divisor = ENC_LINE_PER_REV / ((uint32_t)MOTOR_Z_STEP_PER_REV * McSTEP_Z * Feed_mm / SCREW_Z) /2;
      if (Feed_Divisor < Start_Speed)
      {
         max_OCR5A = Start_Speed;
         if (Step_Z_flag == OFF)
         {
            OCR5A = Start_Speed;
         }
      }
      else
      {
         max_OCR5A = Feed_Divisor;
         OCR5A = Feed_Divisor;
      }
   }
   else
   {
      Feed_Divisor = Start_Speed;
      max_OCR5A = Start_Speed;
      OCR5A = Start_Speed;
   }
   
   Brake_Compens = max_OCR5A - Feed_Divisor + 1;
   Limit_Pos = Limit_Pos_Left - Brake_Compens;

   Motor_Z_Dir = ZCW;
   Motor_Z_CW();
   if (Read_Z_Ena_State == false) Motor_Z_Enable();

   if      (Mode == Mode_Cone_L) {Motor_X_Dir = CW; Motor_X_CW();}
   else if (Mode == Mode_Cone_R) {Motor_X_Dir = CCW; Motor_X_CCW();}
   if (Read_X_Ena_State == false) Motor_X_Enable();
   
   feed_Z_flag = ON;
   Step_X_flag = ON;
   Joy_Z_flag = ON;
   Ena_INT_Z_Feed();
}

void Cone_Right(bool & a_flag, bool & b_flag)
{
   if (b_flag == true) return;
   a_flag = false;
   b_flag = true; 
   
   Cs_Divisor = Cone_Info[Cone_Step].Cs_Div;
   Cm_Divisor = Cone_Info[Cone_Step].Cm_Div;

   Start_Speed = ENC_LINE_PER_REV / ((uint32_t)MOTOR_Z_STEP_PER_REV * McSTEP_Z * MIN_FEED / SCREW_Z) / FEED_ACCEL /2;
   if (Motor_Z_Pos > Limit_Pos_Right + Start_Speed * 2)
   {
      Feed_Divisor = ENC_LINE_PER_REV / ((uint32_t)MOTOR_Z_STEP_PER_REV * McSTEP_Z * Feed_mm / SCREW_Z) /2;
      if (Feed_Divisor < Start_Speed)
      {
         max_OCR5A = Start_Speed;
         if (Step_Z_flag == OFF)
         {
            OCR5A = Start_Speed;
         }
      }
      else
      {
         max_OCR5A = Feed_Divisor;
         OCR5A = Feed_Divisor;
      }
   }
   else
   {
      Feed_Divisor = Start_Speed;
      max_OCR5A = Start_Speed;
      OCR5A = Start_Speed;
   }
   
   Brake_Compens = max_OCR5A - Feed_Divisor + 1;
   Limit_Pos = Limit_Pos_Right + Brake_Compens;

   Motor_Z_Dir = ZCCW;
   Motor_Z_CCW();
   if (Read_Z_Ena_State == false) Motor_Z_Enable();

   if      (Mode == Mode_Cone_L) {Motor_X_Dir = CCW; Motor_X_CCW();}
   else if (Mode == Mode_Cone_R) {Motor_X_Dir = CW; Motor_X_CW();}
   if (Read_X_Ena_State == false) Motor_X_Enable();
   
   feed_Z_flag = ON;
   Step_X_flag = ON;
   Joy_Z_flag = ON;
   Ena_INT_Z_Feed();
}

void Cone_Front(bool & a_flag, bool & b_flag)
{
   if (b_flag == true) return;
   a_flag = false;
   b_flag = true;
}

void Cone_Rear(bool & a_flag, bool & b_flag)
{
   if (b_flag == true) return;
   a_flag = false;
   b_flag = true; 
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ********** "Cone! Fast Feed" mode ********** ///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Rapid_Cone_Left(bool & a_flag, bool & b_flag)
{
   if (a_flag == true) return;
   b_flag = false;
   a_flag = true;
   
   Brake_Compens = (MIN_RAPID_MOTION - MAX_RAPID_MOTION) * REPEAt + 1;
   Limit_Pos = Limit_Pos_Left - Brake_Compens;
   
   Cs_Count = 0;
   Cm_Count = 0;            

   Motor_Z_Dir = ZCW;
   Motor_Z_CW();
   if (Read_Z_Ena_State == false) Motor_Z_Enable();

   if      (Mode == Mode_Cone_L) {Motor_X_Dir = CW; Motor_X_CW();}
   else if (Mode == Mode_Cone_R) {Motor_X_Dir = CCW; Motor_X_CCW();}
   if (Read_X_Ena_State == false) Motor_X_Enable();
   
   Joy_Z_flag = ON;
   rapid_Z_flag = ON;
   Step_X_flag = ON;
   Ena_INT_Z_Rapid();
}

void Rapid_Cone_Right(bool & a_flag, bool & b_flag)
{
   if (a_flag == true) return;
   b_flag = false;
   a_flag = true;
   
   Brake_Compens = (MIN_RAPID_MOTION - MAX_RAPID_MOTION) * REPEAt + 1;
   Limit_Pos = Limit_Pos_Right + Brake_Compens;
   
   Cs_Count = 0;
   Cm_Count = 0;            

   Motor_Z_Dir = ZCCW;
   Motor_Z_CCW();
   if (Read_Z_Ena_State == false) Motor_Z_Enable();

   if      (Mode == Mode_Cone_L) {Motor_X_Dir = CCW; Motor_X_CCW();}
   else if (Mode == Mode_Cone_R) {Motor_X_Dir = CW; Motor_X_CW();}
   if (Read_X_Ena_State == false) Motor_X_Enable();
   
   Joy_Z_flag = ON;
   rapid_Z_flag = ON;
   Step_X_flag = ON;
   Ena_INT_Z_Rapid();  
}
