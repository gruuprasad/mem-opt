; ModuleID = 'batch_process_test1.c'
source_filename = "batch_process_test1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [10 x i8] c"batch_arg\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [22 x i8] c"batch_process_test1.c\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [16 x i8] c"tas_batch_maker\00", section "llvm.metadata"
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (i32 (i32, i32*, i32*, i32)* @singlularFn to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.2, i32 0, i32 0), i32 44 }], section "llvm.metadata"

declare dso_local i32 @printf(i8*, ...) #1

; Function Attrs: nounwind uwtable
define dso_local i32 @singlularFn(i32 %c, i32* %b, i32* %a, i32 %n) #0 {
entry:
  %c.addr = alloca i32, align 4
  %b.addr = alloca i32*, align 8
  %a.addr = alloca i32*, align 8
  %n.addr = alloca i32, align 4
  store i32 %c, i32* %c.addr, align 4, !tbaa !2
  store i32* %b, i32** %b.addr, align 8, !tbaa !6
  %b.addr1 = bitcast i32** %b.addr to i8*
  call void @llvm.var.annotation(i8* %b.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.2, i32 0, i32 0), i32 44)
  %a.addr1 = bitcast i32** %a.addr to i8*
  call void @llvm.var.annotation(i8* %a.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i32 0, i32 0), i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.2, i32 0, i32 0), i32 44)
  store i32* %a, i32** %a.addr, align 8, !tbaa !6
  store i32 %n, i32* %n.addr, align 4, !tbaa !2
  %0 = load i32, i32* %n.addr, align 4, !tbaa !2
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %0)
  %1 = load i32, i32* %c.addr, align 4, !tbaa !2
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %1)
  %2 = load i32, i32* %c.addr, align 4, !tbaa !2
  %add = add nsw i32 %2, 10
  store i32 %add, i32* %c.addr, align 4, !tbaa !2
  %3 = load i32*, i32** %b.addr, align 8, !tbaa !6
  store i32 10, i32* %3, align 4, !tbaa !2
  %4 = load i32*, i32** %a.addr, align 8, !tbaa !6
  %5 = load i32, i32* %4, align 4, !tbaa !2
  %cmp = icmp sgt i32 %5, 10
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %6 = load i32*, i32** %a.addr, align 8, !tbaa !6
  %7 = load i32, i32* %6, align 4, !tbaa !2
  %add3 = add nsw i32 %7, 10
  %8 = load i32*, i32** %a.addr, align 8, !tbaa !6
  store i32 %add3, i32* %8, align 4, !tbaa !2
  br label %if.end

if.else:                                          ; preds = %entry
  %9 = load i32*, i32** %a.addr, align 8, !tbaa !6
  %10 = load i32, i32* %9, align 4, !tbaa !2
  %add4 = add nsw i32 %10, 20
  %11 = load i32*, i32** %a.addr, align 8, !tbaa !6
  store i32 %add4, i32* %11, align 4, !tbaa !2
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %12 = load i32, i32* %c.addr, align 4, !tbaa !2
  %add5 = add nsw i32 %12, 20
  store i32 %add5, i32* %c.addr, align 4, !tbaa !2
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #3

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (trunk 341799)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!7, !7, i64 0}
!7 = !{!"any pointer", !4, i64 0}
