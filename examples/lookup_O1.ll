; ModuleID = 'lookup.c'
source_filename = "lookup.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.packet = type { i32, i64 }

@hashes = common dso_local local_unnamed_addr global [8192 x i32] zeroinitializer, align 16
@packet_buffer = common dso_local local_unnamed_addr global [48 x %struct.packet] zeroinitializer, align 16
@.str = private unnamed_addr constant [10 x i8] c"expensive\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [9 x i8] c"lookup.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.3 = private unnamed_addr constant [10 x i8] c"access %d\00", align 1
@.str.5 = private unnamed_addr constant [10 x i8] c"tas_batch\00", section "llvm.metadata"
@expected = common dso_local local_unnamed_addr global [48 x i32] zeroinitializer, align 16
@llvm.global.annotations = appending global [3 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.packet*)* @hash_lookup_loopSplit to i8*), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 44 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.packet*)* @hash_lookup_v2 to i8*), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 59 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.packet**, i32)* @hash_lookup to i8*), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i32 0, i32 0), i32 73 }], section "llvm.metadata"
@str.7 = private unnamed_addr constant [7 x i8] c"loop 2\00", align 1

; Function Attrs: norecurse nounwind uwtable writeonly
define dso_local void @fill_hashTable() local_unnamed_addr #0 {
entry:
  br label %vector.body

vector.body:                                      ; preds = %vector.body, %entry
  %index = phi i64 [ 0, %entry ], [ %index.next.3, %vector.body ]
  %vec.ind7 = phi <4 x i32> [ <i32 0, i32 1, i32 2, i32 3>, %entry ], [ %vec.ind.next10.3, %vector.body ]
  %0 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %index
  %step.add8 = add <4 x i32> %vec.ind7, <i32 4, i32 4, i32 4, i32 4>
  %1 = bitcast i32* %0 to <4 x i32>*
  store <4 x i32> %vec.ind7, <4 x i32>* %1, align 16, !tbaa !2
  %2 = getelementptr inbounds i32, i32* %0, i64 4
  %3 = bitcast i32* %2 to <4 x i32>*
  store <4 x i32> %step.add8, <4 x i32>* %3, align 16, !tbaa !2
  %index.next = or i64 %index, 8
  %vec.ind.next10 = add <4 x i32> %vec.ind7, <i32 8, i32 8, i32 8, i32 8>
  %4 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %index.next
  %step.add8.1 = add <4 x i32> %vec.ind7, <i32 12, i32 12, i32 12, i32 12>
  %5 = bitcast i32* %4 to <4 x i32>*
  store <4 x i32> %vec.ind.next10, <4 x i32>* %5, align 16, !tbaa !2
  %6 = getelementptr inbounds i32, i32* %4, i64 4
  %7 = bitcast i32* %6 to <4 x i32>*
  store <4 x i32> %step.add8.1, <4 x i32>* %7, align 16, !tbaa !2
  %index.next.1 = or i64 %index, 16
  %vec.ind.next10.1 = add <4 x i32> %vec.ind7, <i32 16, i32 16, i32 16, i32 16>
  %8 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %index.next.1
  %step.add8.2 = add <4 x i32> %vec.ind7, <i32 20, i32 20, i32 20, i32 20>
  %9 = bitcast i32* %8 to <4 x i32>*
  store <4 x i32> %vec.ind.next10.1, <4 x i32>* %9, align 16, !tbaa !2
  %10 = getelementptr inbounds i32, i32* %8, i64 4
  %11 = bitcast i32* %10 to <4 x i32>*
  store <4 x i32> %step.add8.2, <4 x i32>* %11, align 16, !tbaa !2
  %index.next.2 = or i64 %index, 24
  %vec.ind.next10.2 = add <4 x i32> %vec.ind7, <i32 24, i32 24, i32 24, i32 24>
  %12 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %index.next.2
  %step.add8.3 = add <4 x i32> %vec.ind7, <i32 28, i32 28, i32 28, i32 28>
  %13 = bitcast i32* %12 to <4 x i32>*
  store <4 x i32> %vec.ind.next10.2, <4 x i32>* %13, align 16, !tbaa !2
  %14 = getelementptr inbounds i32, i32* %12, i64 4
  %15 = bitcast i32* %14 to <4 x i32>*
  store <4 x i32> %step.add8.3, <4 x i32>* %15, align 16, !tbaa !2
  %index.next.3 = add nuw nsw i64 %index, 32
  %vec.ind.next10.3 = add <4 x i32> %vec.ind7, <i32 32, i32 32, i32 32, i32 32>
  %16 = icmp eq i64 %index.next.3, 8192
  br i1 %16, label %for.cond.cleanup, label %vector.body, !llvm.loop !6

for.cond.cleanup:                                 ; preds = %vector.body
  ret void
}

; Function Attrs: norecurse nounwind uwtable writeonly
define dso_local void @fill_packet_buffer(%struct.packet** nocapture readnone %pkts, i32 %n) local_unnamed_addr #0 {
entry:
  %cmp8 = icmp sgt i32 %n, 0
  br i1 %cmp8, label %for.body.preheader, label %for.cond.cleanup

for.body.preheader:                               ; preds = %entry
  %wide.trip.count = zext i32 %n to i64
  %xtraiter = and i64 %wide.trip.count, 1
  %0 = icmp eq i32 %n, 1
  br i1 %0, label %for.cond.cleanup.loopexit.unr-lcssa, label %for.body.preheader.new

for.body.preheader.new:                           ; preds = %for.body.preheader
  %unroll_iter = sub nsw i64 %wide.trip.count, %xtraiter
  br label %for.body

for.cond.cleanup.loopexit.unr-lcssa:              ; preds = %for.body, %for.body.preheader
  %indvars.iv.unr = phi i64 [ 0, %for.body.preheader ], [ %indvars.iv.next.1, %for.body ]
  %lcmp.mod = icmp eq i64 %xtraiter, 0
  br i1 %lcmp.mod, label %for.cond.cleanup, label %for.body.epil

for.body.epil:                                    ; preds = %for.cond.cleanup.loopexit.unr-lcssa
  %id.epil = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv.unr, i32 0
  %1 = trunc i64 %indvars.iv.unr to i32
  store i32 %1, i32* %id.epil, align 16, !tbaa !8
  %size.epil = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv.unr, i32 1
  store i64 32, i64* %size.epil, align 8, !tbaa !11
  br label %for.cond.cleanup

for.cond.cleanup:                                 ; preds = %for.body.epil, %for.cond.cleanup.loopexit.unr-lcssa, %entry
  ret void

for.body:                                         ; preds = %for.body, %for.body.preheader.new
  %indvars.iv = phi i64 [ 0, %for.body.preheader.new ], [ %indvars.iv.next.1, %for.body ]
  %niter = phi i64 [ %unroll_iter, %for.body.preheader.new ], [ %niter.nsub.1, %for.body ]
  %id = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv, i32 0
  %2 = trunc i64 %indvars.iv to i32
  store i32 %2, i32* %id, align 16, !tbaa !8
  %size = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv, i32 1
  store i64 32, i64* %size, align 8, !tbaa !11
  %indvars.iv.next = or i64 %indvars.iv, 1
  %id.1 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv.next, i32 0
  %3 = trunc i64 %indvars.iv.next to i32
  store i32 %3, i32* %id.1, align 16, !tbaa !8
  %size.1 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv.next, i32 1
  store i64 32, i64* %size.1, align 8, !tbaa !11
  %indvars.iv.next.1 = add nuw nsw i64 %indvars.iv, 2
  %niter.nsub.1 = add i64 %niter, -2
  %niter.ncmp.1 = icmp eq i64 %niter.nsub.1, 0
  br i1 %niter.ncmp.1, label %for.cond.cleanup.loopexit.unr-lcssa, label %for.body
}

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local void @test_loop_function() local_unnamed_addr #1 {
entry:
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @hash_lookup_loopSplit(%struct.packet* %pkt) #2 {
entry:
  %pkt.addr = alloca %struct.packet*, align 8
  store %struct.packet* %pkt, %struct.packet** %pkt.addr, align 8, !tbaa !12
  %pkt.addr1 = bitcast %struct.packet** %pkt.addr to i8*
  call void @llvm.var.annotation(i8* nonnull %pkt.addr1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i64 0, i64 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.1, i64 0, i64 0), i32 44)
  %0 = load %struct.packet*, %struct.packet** %pkt.addr, align 8, !tbaa !12
  %id21 = getelementptr inbounds %struct.packet, %struct.packet* %0, i64 0, i32 0
  %1 = load i32, i32* %id21, align 8, !tbaa !8
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.body
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.3, i64 0, i64 0), i32 %5)
  %puts = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.1 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.2 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.3 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.4 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.5 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.6 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.7 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.8 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.9 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.10 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.11 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.12 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.13 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.14 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.15 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.16 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.17 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.18 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.19 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.20 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.21 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.22 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.23 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.24 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.25 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.26 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.27 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.28 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.29 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.30 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.31 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.32 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.33 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.34 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.35 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.36 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.37 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.38 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.39 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.40 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.41 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.42 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.43 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.44 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.45 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.46 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.47 = call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  ret void

for.body:                                         ; preds = %for.body, %entry
  %2 = phi i32 [ %1, %entry ], [ %5, %for.body ]
  %i.022 = phi i32 [ 0, %entry ], [ %inc, %for.body ]
  %idxprom = sext i32 %2 to i64
  %arrayidx = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %idxprom
  %3 = load i32, i32* %arrayidx, align 4, !tbaa !2
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), i32 %3)
  %inc = add nuw nsw i32 %i.022, 1
  %4 = load %struct.packet*, %struct.packet** %pkt.addr, align 8, !tbaa !12
  %id = getelementptr inbounds %struct.packet, %struct.packet* %4, i64 0, i32 0
  %5 = load i32, i32* %id, align 8, !tbaa !8
  %exitcond = icmp eq i32 %inc, 48
  br i1 %exitcond, label %for.cond.cleanup, label %for.body
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #3

; Function Attrs: nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #4

; Function Attrs: nounwind uwtable
define dso_local void @hash_lookup_v2(%struct.packet* nocapture readonly %pkt) #2 {
entry:
  %id = getelementptr inbounds %struct.packet, %struct.packet* %pkt, i64 0, i32 0
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.body
  ret void

for.body:                                         ; preds = %for.body, %entry
  %i.017 = phi i32 [ 0, %entry ], [ %inc8, %for.body ]
  %0 = load i32, i32* %id, align 8, !tbaa !8
  %idxprom = sext i32 %0 to i64
  %arrayidx = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %idxprom
  %1 = load i32, i32* %arrayidx, align 4, !tbaa !2
  %call = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), i32 %1)
  %puts = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.1 = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.2 = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.3 = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.4 = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.5 = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.6 = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.7 = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.8 = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %puts.9 = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %inc8 = add nuw nsw i32 %i.017, 1
  %exitcond = icmp eq i32 %inc8, 48
  br i1 %exitcond, label %for.cond.cleanup, label %for.body
}

; Function Attrs: nounwind uwtable
define dso_local void @hash_lookup(%struct.packet** nocapture readonly %pkt_buf, i32 %n) #2 {
entry:
  %cmp8 = icmp sgt i32 %n, 0
  br i1 %cmp8, label %for.body.preheader, label %for.cond.cleanup

for.body.preheader:                               ; preds = %entry
  %wide.trip.count = zext i32 %n to i64
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.body, %entry
  ret void

for.body:                                         ; preds = %for.body, %for.body.preheader
  %indvars.iv = phi i64 [ 0, %for.body.preheader ], [ %indvars.iv.next, %for.body ]
  %arrayidx = getelementptr inbounds %struct.packet*, %struct.packet** %pkt_buf, i64 %indvars.iv
  %0 = load %struct.packet*, %struct.packet** %arrayidx, align 8, !tbaa !12
  %id = getelementptr inbounds %struct.packet, %struct.packet* %0, i64 0, i32 0
  %1 = load i32, i32* %id, align 8, !tbaa !8
  %idxprom1 = sext i32 %1 to i64
  %arrayidx2 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %idxprom1
  %2 = load i32, i32* %arrayidx2, align 4, !tbaa !2
  %call = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), i32 %2)
  %puts = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0))
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, %wide.trip.count
  br i1 %exitcond, label %for.cond.cleanup, label %for.body
}

; Function Attrs: nounwind uwtable
define dso_local i32 @main() local_unnamed_addr #2 {
entry:
  %call = tail call noalias i8* @malloc(i64 384) #3
  %0 = bitcast i8* %call to %struct.packet**
  br label %for.body

for.body.i32:                                     ; preds = %vector.body, %for.body.i32
  %indvars.iv.i28 = phi i64 [ %indvars.iv.next.i30.2, %for.body.i32 ], [ 0, %vector.body ]
  %id.i29 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv.i28, i32 0
  %1 = trunc i64 %indvars.iv.i28 to i32
  store i32 %1, i32* %id.i29, align 16, !tbaa !8
  %size.i = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv.i28, i32 1
  store i64 32, i64* %size.i, align 8, !tbaa !11
  %indvars.iv.next.i30 = add nuw nsw i64 %indvars.iv.i28, 1
  %id.i29.1 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv.next.i30, i32 0
  %2 = trunc i64 %indvars.iv.next.i30 to i32
  store i32 %2, i32* %id.i29.1, align 16, !tbaa !8
  %size.i.1 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv.next.i30, i32 1
  store i64 32, i64* %size.i.1, align 8, !tbaa !11
  %indvars.iv.next.i30.1 = add nuw nsw i64 %indvars.iv.i28, 2
  %id.i29.2 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv.next.i30.1, i32 0
  %3 = trunc i64 %indvars.iv.next.i30.1 to i32
  store i32 %3, i32* %id.i29.2, align 16, !tbaa !8
  %size.i.2 = getelementptr inbounds [48 x %struct.packet], [48 x %struct.packet]* @packet_buffer, i64 0, i64 %indvars.iv.next.i30.1, i32 1
  store i64 32, i64* %size.i.2, align 8, !tbaa !11
  %indvars.iv.next.i30.2 = add nuw nsw i64 %indvars.iv.i28, 3
  %exitcond.i31.2 = icmp eq i64 %indvars.iv.next.i30.2, 48
  br i1 %exitcond.i31.2, label %for.body.i27, label %for.body.i32

for.body.i27:                                     ; preds = %for.body.i32, %for.body.i27
  %indvars.iv.i23 = phi i64 [ %indvars.iv.next.i25, %for.body.i27 ], [ 0, %for.body.i32 ]
  %arrayidx.i24 = getelementptr inbounds %struct.packet*, %struct.packet** %0, i64 %indvars.iv.i23
  %4 = load %struct.packet*, %struct.packet** %arrayidx.i24, align 8, !tbaa !12
  %id.i = getelementptr inbounds %struct.packet, %struct.packet* %4, i64 0, i32 0
  %5 = load i32, i32* %id.i, align 8, !tbaa !8
  %idxprom1.i = sext i32 %5 to i64
  %arrayidx2.i = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %idxprom1.i
  %6 = load i32, i32* %arrayidx2.i, align 4, !tbaa !2
  %call.i = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), i32 %6) #3
  %puts.i = tail call i32 @puts(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i64 0, i64 0)) #3
  %indvars.iv.next.i25 = add nuw nsw i64 %indvars.iv.i23, 1
  %exitcond.i26 = icmp eq i64 %indvars.iv.next.i25, 48
  br i1 %exitcond.i26, label %for.body6, label %for.body.i27

for.body:                                         ; preds = %for.body, %entry
  %indvars.iv35 = phi i64 [ 0, %entry ], [ %indvars.iv.next36, %for.body ]
  %call1 = tail call noalias i8* @malloc(i64 16) #3
  %arrayidx = getelementptr inbounds %struct.packet*, %struct.packet** %0, i64 %indvars.iv35
  %7 = bitcast %struct.packet** %arrayidx to i8**
  store i8* %call1, i8** %7, align 8, !tbaa !12
  %indvars.iv.next36 = add nuw nsw i64 %indvars.iv35, 1
  %exitcond37 = icmp eq i64 %indvars.iv.next36, 48
  br i1 %exitcond37, label %vector.body, label %for.body

vector.body:                                      ; preds = %for.body, %vector.body
  %index = phi i64 [ %index.next.3, %vector.body ], [ 0, %for.body ]
  %vec.ind39 = phi <4 x i32> [ %vec.ind.next42.3, %vector.body ], [ <i32 0, i32 1, i32 2, i32 3>, %for.body ]
  %8 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %index
  %step.add40 = add <4 x i32> %vec.ind39, <i32 4, i32 4, i32 4, i32 4>
  %9 = bitcast i32* %8 to <4 x i32>*
  store <4 x i32> %vec.ind39, <4 x i32>* %9, align 16, !tbaa !2
  %10 = getelementptr inbounds i32, i32* %8, i64 4
  %11 = bitcast i32* %10 to <4 x i32>*
  store <4 x i32> %step.add40, <4 x i32>* %11, align 16, !tbaa !2
  %index.next = or i64 %index, 8
  %vec.ind.next42 = add <4 x i32> %vec.ind39, <i32 8, i32 8, i32 8, i32 8>
  %12 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %index.next
  %step.add40.1 = add <4 x i32> %vec.ind39, <i32 12, i32 12, i32 12, i32 12>
  %13 = bitcast i32* %12 to <4 x i32>*
  store <4 x i32> %vec.ind.next42, <4 x i32>* %13, align 16, !tbaa !2
  %14 = getelementptr inbounds i32, i32* %12, i64 4
  %15 = bitcast i32* %14 to <4 x i32>*
  store <4 x i32> %step.add40.1, <4 x i32>* %15, align 16, !tbaa !2
  %index.next.1 = or i64 %index, 16
  %vec.ind.next42.1 = add <4 x i32> %vec.ind39, <i32 16, i32 16, i32 16, i32 16>
  %16 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %index.next.1
  %step.add40.2 = add <4 x i32> %vec.ind39, <i32 20, i32 20, i32 20, i32 20>
  %17 = bitcast i32* %16 to <4 x i32>*
  store <4 x i32> %vec.ind.next42.1, <4 x i32>* %17, align 16, !tbaa !2
  %18 = getelementptr inbounds i32, i32* %16, i64 4
  %19 = bitcast i32* %18 to <4 x i32>*
  store <4 x i32> %step.add40.2, <4 x i32>* %19, align 16, !tbaa !2
  %index.next.2 = or i64 %index, 24
  %vec.ind.next42.2 = add <4 x i32> %vec.ind39, <i32 24, i32 24, i32 24, i32 24>
  %20 = getelementptr inbounds [8192 x i32], [8192 x i32]* @hashes, i64 0, i64 %index.next.2
  %step.add40.3 = add <4 x i32> %vec.ind39, <i32 28, i32 28, i32 28, i32 28>
  %21 = bitcast i32* %20 to <4 x i32>*
  store <4 x i32> %vec.ind.next42.2, <4 x i32>* %21, align 16, !tbaa !2
  %22 = getelementptr inbounds i32, i32* %20, i64 4
  %23 = bitcast i32* %22 to <4 x i32>*
  store <4 x i32> %step.add40.3, <4 x i32>* %23, align 16, !tbaa !2
  %index.next.3 = add nuw nsw i64 %index, 32
  %vec.ind.next42.3 = add <4 x i32> %vec.ind39, <i32 32, i32 32, i32 32, i32 32>
  %24 = icmp eq i64 %index.next.3, 8192
  br i1 %24, label %for.body.i32, label %vector.body, !llvm.loop !14

for.cond.cleanup5:                                ; preds = %for.body6
  tail call void @free(i8* nonnull %call) #3
  ret i32 0

for.body6:                                        ; preds = %for.body.i27, %for.body6
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body6 ], [ 0, %for.body.i27 ]
  %arrayidx8 = getelementptr inbounds %struct.packet*, %struct.packet** %0, i64 %indvars.iv
  %25 = bitcast %struct.packet** %arrayidx8 to i8**
  %26 = load i8*, i8** %25, align 8, !tbaa !12
  tail call void @free(i8* %26) #3
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond = icmp eq i64 %indvars.iv.next, 48
  br i1 %exitcond, label %for.cond.cleanup5, label %for.body6
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) local_unnamed_addr #4

; Function Attrs: nounwind
declare dso_local void @free(i8* nocapture) local_unnamed_addr #4

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #3

attributes #0 = { norecurse nounwind uwtable writeonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }
attributes #4 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (trunk 341799)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.isvectorized", i32 1}
!8 = !{!9, !3, i64 0}
!9 = !{!"packet", !3, i64 0, !10, i64 8}
!10 = !{!"long", !4, i64 0}
!11 = !{!9, !10, i64 8}
!12 = !{!13, !13, i64 0}
!13 = !{!"any pointer", !4, i64 0}
!14 = distinct !{!14, !7}
