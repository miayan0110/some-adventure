#include "stdafx.h"
#include "myfunc.h"

/* Getter */
int MyBitmap::GetRight() {
	return locations[frameIndex].right;
}

int MyBitmap::GetBottom() {
	return locations[frameIndex].bottom;
}

double MyBitmap::CenterX() {
	return 0.5*(locations[frameIndex].left + locations[frameIndex].right);
}

double MyBitmap::CenterY() {
	return 0.5*(locations[frameIndex].top + locations[frameIndex].bottom);
}

/*Is Function */
bool MyBitmap::IsEaten(MyBitmap mb1, MyBitmap mb2) {
	if (mb1.CenterX() >= mb2.GetLeft()
		&& mb1.CenterX() <= mb2.GetRight()
		&& mb1.CenterY() >= mb2.GetTop()
		&& mb1.CenterY() <= mb2.GetBottom()) {
		return true;
	}
	return false;
}

/* Animation */
void MyBitmap::StopKeepAni(bool start) {
	isAnimation = start;
}