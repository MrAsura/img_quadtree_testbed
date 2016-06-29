% Test image quads
mex imageQuads.c
%%

%pic = 'pic.jpg';
%dim = 256;
pic = 'pic.png';
dim = 1024;

img = imread(pic);
%img = rgb2gray(img);
img = cast(img,'double')./225;
img2 = img(1:dim,1:dim,:);
img2r = img(1:dim,1:dim,1);
img2g = img(1:dim,1:dim,2);
img2b = img(1:dim,1:dim,3);

[ar,br,cr,dr,er,fr,rr] = imageQuads(img2r);
[ab,bb,cb,db,eb,fb,rb] = imageQuads(img2b);
[ag,bg,cg,dg,eg,fg,rg] = imageQuads(img2g);

a = zeros(dim,dim,3);
a(:,:,1) = ar;
a(:,:,2) = ag;
a(:,:,3) = ab;

b = zeros(dim,dim,3);
b(:,:,1) = br;
b(:,:,2) = bg;
b(:,:,3) = bb;

c = zeros(dim,dim,3);
c(:,:,1) = cr;
c(:,:,2) = cg;
c(:,:,3) = cb;

d = zeros(dim,dim,3);
d(:,:,1) = dr;
d(:,:,2) = dg;
d(:,:,3) = db;

e = zeros(dim,dim,3);
e(:,:,1) = er;
e(:,:,2) = eg;
e(:,:,3) = eb;

f = zeros(dim,dim,3);
f(:,:,1) = fr;
f(:,:,2) = fg;
f(:,:,3) = fb;

%colormap gray;
subplot(3,3,1);
imshow(img2);
title('Orig');
subplot(3,3,2);
imshow(img2);
title('Orig');
subplot(3,3,3);
imshow(img2);
title('Orig');

subplot(3,3,4);
imshow(a);
title(['Low-Avg: ratio=' num2str(rr(1)) ', ' num2str(rg(1)) ', ' num2str(rb(1))]);

subplot(3,3,5);
imshow(b);
title(['Med-Avg: ratio=' num2str(rr(2)) ', ' num2str(rg(2)) ', ' num2str(rb(2))]);

subplot(3,3,6);
imshow(c);
title(['High-Avg: ratio=' num2str(rr(3)) ', ' num2str(rg(3)) ', ' num2str(rb(3))]);

subplot(3,3,7);
imshow(d);
title(['Low-Max: ratio=' num2str(rr(4)) ', ' num2str(rg(4)) ', ' num2str(rb(4))]);

subplot(3,3,8);
imshow(e);
title(['Med-Max: ratio=' num2str(rr(5)) ', ' num2str(rg(5)) ', ' num2str(rb(5))]);

subplot(3,3,9);
imshow(f);
title(['High-Max: ratio=' num2str(rr(6)) ', ' num2str(rg(6)) ', ' num2str(rb(6))]);