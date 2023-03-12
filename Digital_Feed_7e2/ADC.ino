void Read_ADC_Feed()
{
   /////////// Feed Variable ///////////
   if (Mode == Mode_Feed || Mode == Mode_Cone_L || Mode == Mode_Cone_R || Mode == Mode_aFeed || Mode == Mode_Sphere)
   {
      int New_ADC_Feed = analogRead(A7);
      if (New_ADC_Feed > ADC_Feed +4 || New_ADC_Feed < ADC_Feed -4)
      {
         if (++x > 15) {x = 0;}
         Sum_ADC = Sum_ADC - ADC_Array[x];
         ADC_Array[x] = New_ADC_Feed;
         Sum_ADC = Sum_ADC + New_ADC_Feed;
         ADC_Feed = Sum_ADC /16;
      }
   }

   if (Mode == Mode_Feed || Mode == Mode_Cone_L || Mode == Mode_Cone_R || Mode == Mode_Sphere)
   {
      uint16_t Feed_mm_New = MAX_FEED - long(MAX_FEED - MIN_FEED + 1) * ADC_Feed / 1024;
      if (Feed_mm_New != Feed_mm)
      {
         Feed_mm = Feed_mm_New;
         Print();
         Beep();

         switch (Mode)
         {
            case Mode_Feed:    ////////////////////////////////
            if (Joy_Z_flag == ON && Button_Rapid != 0 && Step_Z_flag == ON)
            {
               b_flag = false;
               if (Motor_Z_Dir == ZCW) {Feed_Left(a_flag, b_flag);}
               else                   {Feed_Right(a_flag, b_flag);}
            }
            else if (Joy_X_flag == ON && Button_Rapid != 0 && Step_X_flag == ON)
            {
               b_flag = false;
               if (Motor_X_Dir == CW) {Feed_Front(a_flag, b_flag);}
               else                   {Feed_Rear(a_flag, b_flag);} 
            }
            break;

            case Mode_Cone_L:    ////////////////////////////////
            case Mode_Cone_R:    ////////////////////////////////
            if (Joy_Z_flag == ON && Button_Rapid != 0  && Step_Z_flag == ON)
            {
               b_flag = false;
               if (Motor_Z_Dir == ZCW) {Cone_Left(a_flag, b_flag);}
               else                   {Cone_Right(a_flag, b_flag);}
            }
            else if (Joy_X_flag == ON && Button_Rapid != 0 && Step_X_flag == ON)
            {
               b_flag = false;
               if (Motor_X_Dir == CW) {Feed_Front(a_flag, b_flag);}
               else                   {Feed_Rear(a_flag, b_flag);}
            }
            break;
         }
      }
   }

   else if (Mode == Mode_aFeed)
   {
      uint16_t aFeed_mm_New = MAX_aFEED/10 - long(MAX_aFEED/10 - MIN_aFEED/10 + 1) * ADC_Feed / 1024;
      aFeed_mm_New = (aFeed_mm_New * 10);
      if (aFeed_mm_New != aFeed_mm)
      {
         aFeed_mm = aFeed_mm_New;
         Print();
         Beep();

         switch (Mode)
         {
            case Mode_aFeed:     //////////////////////////////////
            if (Joy_Z_flag == ON && Button_Rapid != 0  && Step_Z_flag == ON)
            {
               b_flag = false;
               if (Motor_Z_Dir == ZCW) {aFeed_Left(a_flag, b_flag);}
               else                   {aFeed_Right(a_flag, b_flag);}
            }
            else if (Joy_X_flag == ON && Button_Rapid != 0  && Step_X_flag == ON)
            {
               b_flag = false;
               if (Motor_X_Dir == CW) {aFeed_Front(a_flag, b_flag);}
               else                   {aFeed_Rear(a_flag, b_flag);}
            }
            break;
         }
      } 
   } 
}
