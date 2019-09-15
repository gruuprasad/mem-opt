; ModuleID = 'batchmaker_test2.c'
source_filename = "batchmaker_test2.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.packet = type { i32, i32, i32, i32 }

@.str = private unnamed_addr constant [10 x i8] c"batch_arg\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [19 x i8] c"batchmaker_test2.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [16 x i8] c"packet ip = %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [16 x i8] c"packet id = %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [9 x i8] c"fastpath\00", align 1
@.str.5 = private unnamed_addr constant [9 x i8] c"slowpath\00", align 1
@.str.6 = private unnamed_addr constant [7 x i8] c"unlock\00", align 1
@.str.7 = private unnamed_addr constant [16 x i8] c"tas_batch_maker\00", section "llvm.metadata"
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (i32 (%struct.packet*)* @process_packet to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.7, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 17 }], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @process_packet(%struct.packet*) #0 {
  %2 = alloca i32, align 4
  %3 = alloca %struct.packet*, align 8
  %4 = alloca %struct.packet*, align 8
  store %struct.packet* %0, %struct.packet** %3, align 8
  %5 = bitcast %struct.packet** %3 to i8*
  call void @llvm.var.annotation(i8* %5, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 17)
  %6 = load %struct.packet*, %struct.packet** %3, align 8
  store %struct.packet* %6, %struct.packet** %4, align 8
  %7 = load %struct.packet*, %struct.packet** %4, align 8
  %8 = getelementptr inbounds %struct.packet, %struct.packet* %7, i32 0, i32 0
  %9 = load i32, i32* %8, align 4
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i32 %9)
  %11 = load %struct.packet*, %struct.packet** %4, align 8
  %12 = getelementptr inbounds %struct.packet, %struct.packet* %11, i32 0, i32 1
  %13 = load i32, i32* %12, align 4
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 %13)
  %15 = load %struct.packet*, %struct.packet** %4, align 8
  %16 = getelementptr inbounds %struct.packet, %struct.packet* %15, i32 0, i32 1
  %17 = load i32, i32* %16, align 4
  %18 = icmp eq i32 %17, 1
  br i1 %18, label %19, label %20

; <label>:19:                                     ; preds = %1
  br label %30

; <label>:20:                                     ; preds = %1
  %21 = load %struct.packet*, %struct.packet** %4, align 8
  %22 = getelementptr inbounds %struct.packet, %struct.packet* %21, i32 0, i32 1
  %23 = load i32, i32* %22, align 4
  %24 = icmp eq i32 %23, 2
  br i1 %24, label %25, label %26

; <label>:25:                                     ; preds = %20
  br label %28

; <label>:26:                                     ; preds = %20
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.4, i32 0, i32 0))
  store i32 0, i32* %2, align 4
  br label %32

; <label>:28:                                     ; preds = %25
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.5, i32 0, i32 0))
  store i32 -2, i32* %2, align 4
  br label %32

; <label>:30:                                     ; preds = %19
  %31 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.6, i32 0, i32 0))
  store i32 -1, i32* %2, align 4
  br label %32

; <label>:32:                                     ; preds = %30, %28, %26
  %33 = load i32, i32* %2, align 4
  ret i32 %33
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #1

declare i32 @printf(i8*, ...) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.packet*, align 8
  %3 = alloca i32*, align 8
  store i32 0, i32* %1, align 4
  %4 = call noalias i8* @malloc(i64 16) #1
  %5 = bitcast i8* %4 to %struct.packet*
  store %struct.packet* %5, %struct.packet** %2, align 8
  %6 = load %struct.packet*, %struct.packet** %2, align 8
  %7 = getelementptr inbounds %struct.packet, %struct.packet* %6, i32 0, i32 0
  store i32 1000, i32* %7, align 4
  %8 = load %struct.packet*, %struct.packet** %2, align 8
  %9 = getelementptr inbounds %struct.packet, %struct.packet* %8, i32 0, i32 1
  store i32 1, i32* %9, align 4
  %10 = load %struct.packet*, %struct.packet** %2, align 8
  %11 = getelementptr inbounds %struct.packet, %struct.packet* %10, i32 0, i32 2
  store i32 200, i32* %11, align 4
  %12 = load %struct.packet*, %struct.packet** %2, align 8
  %13 = getelementptr inbounds %struct.packet, %struct.packet* %12, i32 0, i32 3
  store i32 300, i32* %13, align 4
  %14 = load %struct.packet*, %struct.packet** %2, align 8
  %15 = call i32 @process_packet(%struct.packet* %14)
  store i32* null, i32** %3, align 8
  %16 = load i32*, i32** %3, align 8
  call void @process_packet_batch(%struct.packet** %2, i32 1, i32* %16)
  %17 = load %struct.packet*, %struct.packet** %2, align 8
  %18 = bitcast %struct.packet* %17 to i8*
  call void @free(i8* %18) #1
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #3

declare void @process_packet_batch(%struct.packet**, i32, i32*) #2

; Function Attrs: nounwind
declare void @free(i8*) #3

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
