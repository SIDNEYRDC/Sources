% [B,XY] = bucky;
% gplot(A,XY,'-*');

x = 1:2;
y = 1:3;
[XX,YY] = meshgrid(x,y);
a = (rand([2*3,2*3])>0.8);
P=[XX(:),YY(:)];
gplot(a,P,'-*');
axis([0,7,0,7]);