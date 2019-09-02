; ModuleID = 'batchmaker_test1.c'
source_filename = "batchmaker_test1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.packet = type { i32, i32, i32, i32 }

@.str = private unnamed_addr constant [16 x i8] c"tas_batch_maker\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [19 x i8] c"batchmaker_test1.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [10 x i8] c"batch_arg\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [10 x i8] c"expensive\00", section "llvm.metadata"
@llvm.global.annotations = appending global [2 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.packet*)* @test_fn to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 17 }, { i8*, i8*, i8*, i32 } { i8* bitcast (i32 ()* @test_fn2 to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 25 }], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local void @test_fn(%struct.packet*) #0 {
  %2 = alloca %struct.packet*, align 8
  store %struct.packet* %0, %struct.packet** %2, align 8
  %3 = load %struct.packet*, %struct.packet** %2, align 8
  %4 = getelementptr inbounds %struct.packet, %struct.packet* %3, i32 0, i32 0
  store i32 1, i32* %4, align 4
  %5 = load %struct.packet*, %struct.packet** %2, align 8
  %6 = getelementptr inbounds %struct.packet, %struct.packet* %5, i32 0, i32 1
  store i32 10, i32* %6, align 4
  %7 = load %struct.packet*, %struct.packet** %2, align 8
  %8 = getelementptr inbounds %struct.packet, %struct.packet* %7, i32 0, i32 2
  store i32 2000, i32* %8, align 4
  %9 = load %struct.packet*, %struct.packet** %2, align 8
  %10 = getelementptr inbounds %struct.packet, %struct.packet* %9, i32 0, i32 3
  store i32 4000, i32* %10, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @test_fn2() #0 {
  ret i32 0
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @test_fn3(i32*, i32*) #0 {
  %3 = alloca i32*, align 8
  %4 = alloca i32*, align 8
  store i32* %0, i32** %3, align 8
  store i32* %1, i32** %4, align 8
  %5 = bitcast i32** %4 to i8*
  call void @llvm.var.annotation(i8* %5, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 30)
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @test_fn4(i32*, i32*, i32*) #0 {
  %4 = alloca i32*, align 8
  %5 = alloca i32*, align 8
  %6 = alloca i32*, align 8
  store i32* %0, i32** %4, align 8
  store i32* %1, i32** %5, align 8
  %7 = bitcast i32** %5 to i8*
  call void @llvm.var.annotation(i8* %7, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 34)
  store i32* %2, i32** %6, align 8
  %8 = bitcast i32** %6 to i8*
  call void @llvm.var.annotation(i8* %8, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 34)
  ret i32 0
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @test_fn5(i32*, i32*, i32, i32*) #0 {
  %5 = alloca i32*, align 8
  %6 = alloca i32*, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32*, align 8
  store i32* %0, i32** %5, align 8
  store i32* %1, i32** %6, align 8
  %9 = bitcast i32** %6 to i8*
  call void @llvm.var.annotation(i8* %9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 38)
  store i32 %2, i32* %7, align 4
  store i32* %3, i32** %8, align 8
  %10 = bitcast i32** %8 to i8*
  call void @llvm.var.annotation(i8* %10, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 38)
  ret i32 0
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @test_fn6() #0 {
  %1 = alloca i32*, align 8
  %2 = alloca i32*, align 8
  %3 = bitcast i32** %1 to i8*
  call void @llvm.var.annotation(i8* %3, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 44)
  %4 = bitcast i32** %2 to i8*
  call void @llvm.var.annotation(i8* %4, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 45)
  store i32* null, i32** %2, align 8
  store i32* null, i32** %1, align 8
  ret i32 0
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @caller_fn() #0 {
  %1 = alloca %struct.packet*, align 8
  %2 = call noalias i8* @malloc(i64 16) #1
  %3 = bitcast i8* %2 to %struct.packet*
  store %struct.packet* %3, %struct.packet** %1, align 8
  %4 = load %struct.packet*, %struct.packet** %1, align 8
  call void @test_fn(%struct.packet* %4)
  %5 = load %struct.packet*, %struct.packet** %1, align 8
  %6 = bitcast %struct.packet* %5 to i8*
  call void @free(i8* %6) #1
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #2

; Function Attrs: nounwind
declare void @free(i8*) #2

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
