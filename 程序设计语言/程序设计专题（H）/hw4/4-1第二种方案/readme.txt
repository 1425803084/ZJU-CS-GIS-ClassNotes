�汾1��һ���ǳ��򵥵�ʵ��
windows 10 ʹ��vs2017

��igp1��MouseEventProcess�����������޸ģ�

��case button_down��
				  if (inBox(mx, my, textx, textx+TextStringWidth(text), 
				                           texty, texty+GetFontHeight())){
				  	  isMoveText = TRUE;
				  }else
				  isMoveCircle = TRUE;
�� isMoveCircleĬ����Ϊtrue��

  ��       case MOUSEMOVE��
			  if (isMoveCircle) {
                  //SetEraseMode(TRUE);/*����ǰһ��*/
                  //DrawCenteredCircle(ccx, ccy, radius);
				  ccx = mx;
				  ccy = my;
				  omx = mx;
				  omy = my;
				  SetEraseMode(FALSE);/*���µ�*/
                  DrawCenteredCircle(ccx, ccy, radius);

ֱ�ӻ��µ�Բ
ÿ��Բ���������ͳ���һ����