; ModuleID = '../../test/unittests/inputs/ifelse_multi_goto.c'
source_filename = "../../test/unittests/inputs/ifelse_multi_goto.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.packet = type { i32, i32, i32, i32 }

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local i32 @process_packet(%struct.packet*) #0 {
  %2 = alloca i32, align 4
  %3 = alloca %struct.packet*, align 8
  %4 = alloca %struct.packet*, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store %struct.packet* %0, %struct.packet** %3, align 8
  %8 = load %struct.packet*, %struct.packet** %3, align 8
  store %struct.packet* %8, %struct.packet** %4, align 8
  store i32 0, i32* %5, align 4
  store i32 0, i32* %6, align 4
  store i32 0, i32* %7, align 4
  %9 = load %struct.packet*, %struct.packet** %4, align 8
  %10 = getelementptr inbounds %struct.packet, %struct.packet* %9, i32 0, i32 1
  %11 = load i32, i32* %10, align 4
  %12 = icmp eq i32 %11, 1
  br i1 %12, label %13, label %14

; <label>:13:                                     ; preds = %1
  br label %47

; <label>:14:                                     ; preds = %1
  %15 = load %struct.packet*, %struct.packet** %4, align 8
  %16 = getelementptr inbounds %struct.packet, %struct.packet* %15, i32 0, i32 1
  %17 = load i32, i32* %16, align 4
  %18 = icmp eq i32 %17, 2
  br i1 %18, label %19, label %20

; <label>:19:                                     ; preds = %14
  br label %46

; <label>:20:                                     ; preds = %14
  %21 = load i32, i32* %5, align 4
  %22 = icmp eq i32 %21, 0
  br i1 %22, label %23, label %32

; <label>:23:                                     ; preds = %20
  %24 = load i32, i32* %5, align 4
  %25 = add nsw i32 %24, 100
  store i32 %25, i32* %5, align 4
  %26 = load i32, i32* %6, align 4
  %27 = add nsw i32 %26, 200
  store i32 %27, i32* %6, align 4
  %28 = load i32, i32* %5, align 4
  %29 = load i32, i32* %6, align 4
  %30 = add nsw i32 %28, %29
  %31 = add nsw i32 %30, 50
  store i32 %31, i32* %7, align 4
  br label %46

; <label>:32:                                     ; preds = %20
  %33 = load i32, i32* %5, align 4
  %34 = icmp sgt i32 %33, 50
  br i1 %34, label %35, label %40

; <label>:35:                                     ; preds = %32
  %36 = load i32, i32* %5, align 4
  %37 = sub nsw i32 %36, 50
  store i32 %37, i32* %5, align 4
  %38 = load i32, i32* %6, align 4
  %39 = sub nsw i32 %38, 100
  store i32 %39, i32* %6, align 4
  br label %45

; <label>:40:                                     ; preds = %32
  %41 = load i32, i32* %5, align 4
  %42 = load i32, i32* %6, align 4
  %43 = add nsw i32 %41, %42
  %44 = add nsw i32 %43, 50
  store i32 %44, i32* %7, align 4
  br label %45

; <label>:45:                                     ; preds = %40, %35
  store i32 0, i32* %2, align 4
  br label %48

; <label>:46:                                     ; preds = %23, %19
  store i32 -2, i32* %2, align 4
  br label %48

; <label>:47:                                     ; preds = %13
  store i32 -1, i32* %2, align 4
  br label %48

; <label>:48:                                     ; preds = %47, %46, %45
  %49 = load i32, i32* %2, align 4
  ret i32 %49
}

attributes #0 = { noinline nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
