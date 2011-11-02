Doctrine でファイルアップロードを扱う方法
=========================================

Doctrine のエンティティでファイルアップロードを扱う方法は、他のファイルアップロードと違いはありません。つまり、同じようにフォーム情報を受け取った後にコントローラでファイルを移動できます。この例は、 :doc:`file type reference</reference/forms/types/file>` ページを見て、その方法を学ぶことができます。

望むのであれば、ファイルアップロードをエンティティのライフサイクルにも統合することができます(作成、変更、削除など)。エンティティが Doctrine から作成、変更、削除がされれば、ファイルアップロードや削除の処理が自動的に行われます(コントローラで何もする必要はありません)。

これを実際に動作させるために、多くの細かいことに注意をする必要がありますが、それについては、このクックブックの記事で説明します。

基本セットアップ
----------------

まず、次のようなシンプルな Doctrine のエンティティを作成してください。

::

    // src/Acme/DemoBundle/Entity/Document.php
    namespace Acme\DemoBundle\Entity;

    use Doctrine\ORM\Mapping as ORM;
    use Symfony\Component\Validator\Constraints as Assert;

    /**
     * @ORM\Entity
     */
    class Document
    {
        /**
         * @ORM\Id
         * @ORM\Column(type="integer")
         * @ORM\GeneratedValue(strategy="AUTO")
         */
        public $id;

        /**
         * @ORM\Column(type="string", length=255)
         * @Assert\NotBlank
         */
        public $name;

        /**
         * @ORM\Column(type="string", length=255, nullable=true)
         */
        public $path;

        public function getAbsolutePath()
        {
            return null === $this->path ? null : $this->getUploadRootDir().'/'.$this->path;
        }

        public function getWebPath()
        {
            return null === $this->path ? null : $this->getUploadDir().'/'.$this->path;
        }

        protected function getUploadRootDir()
        {
            // アップロードされたファイルを保存する場所への絶対パス
            return __DIR__.'/../../../../web/'.$this->getUploadDir();
        }

        protected function getUploadDir()
        {
            // ビューで アップロードされたファイルを参照する際のために __DIR__ を取り除く
            return 'uploads/documents';
        }
    }

``Document`` エンティティは、ファイルに結び付いた ``name`` プロパティを持つことになります。そして、 ``path`` プロパティは、データベースに保存するファイルへの相対パスを格納します。 ``getAbsolutePath()`` は、便利なメソッドでファイルへの絶対パスを返し、 ``getWebPath()`` もまた便利なメソッドでウェブ上でのパスを返します。これは、テンプレートの中で、アップロードされたファイルへのリンクするときなどに使われます。

.. tip::

    まだファイルタイプに関するドキュメント :doc:`file</reference/forms/types/file>` を読んでいなければ、先に読むことをお勧めします。そうすれば、アップロード処理の基本を理解できます。

.. note::

    この例のようにアノテーションルールを指定しているならば、アノテーションによるバリデーションが有効になっていることを確認してください。 :ref:`validation configuration<book-validation-configuration>`.を参照してください。

実際のフォームのファイルアップロードを処理するには、 "virtual" ``file`` フィールドを使用してください。例えば、コントローラ内で直接フォームを作成するのであれば、次のようになります。

::

    public function uploadAction()
    {
        // ...

        $form = $this->createFormBuilder($document)
            ->add('name')
            ->add('file')
            ->getForm()
        ;

        // ...
    }

次に、 ``Document`` クラスにこのプロパティを追加し、バリデーションルールを加えます。

::

    // src/Acme/DemoBundle/Entity/Document.php

    // ...
    class Document
    {
        /**
         * @Assert\File(maxSize="6000000")
         */
        public $file;

        // ...
    }

.. note::

    ``File`` 制約を使用すれば、 Symfony2 は自動的にそのフォームフィールドがファイルアップロードによるものだと判断します。上記でフォーム作成で ``->add('file')`` の際に、明示的にファイルの最大サイズを指定しなかったのは、このためです。

次のコントローラは、アップロードの全ての処理の扱い方になります。

::

    use Acme\DemoBundle\Entity\Document;
    use Sensio\Bundle\FrameworkExtraBundle\Configuration\Template;
    // ...

    /**
     * @Template()
     */
    public function uploadAction()
    {
        $document = new Document();
        $form = $this->createFormBuilder($document)
            ->add('name')
            ->add('file')
            ->getForm()
        ;

        if ($this->getRequest()->getMethod() === 'POST') {
            $form->bindRequest($this->getRequest());
            if ($form->isValid()) {
                $em = $this->getDoctrine()->getEntityManager();

                $em->persist($document);
                $em->flush();

                $this->redirect($this->generateUrl('...'));
            }
        }

        return array('form' => $form->createView());
    }

.. note::

    テンプレートを書く際には、 ``enctype`` 属性を忘れないようにしてください。
    :

    .. code-block:: html+php

        <h1>Upload File</h1>

        <form action="#" method="post" {{ form_enctype(form) }}>
            {{ form_widget(form) }}

            <input type="submit" value="Upload Document" />
        </form>

前のコントローラでは、自動的に送信された名前で ``Document`` エンティティを保存します。しかし、ファイルには何も処理をしていないですし、 ``path`` プロパティにも、何も指定していないので空になります。

ファイルアップロードを扱う簡単な方法は、エンティティを保存する直前に、ファイルを移動し、その移動先に合わせて ``path`` プロパティにセットします。そうするには、まずファイルアップロードの直後に ``Document`` クラスの ``upload()`` メソッドを呼びます。

::

    if ($form->isValid()) {
        $em = $this->getDoctrine()->getEntityManager();

        $document->upload();

        $em->persist($document);
        $em->flush();

        $this->redirect('...');
    }

``upload()`` メソッドは、 ``file`` フィールドを受け取った後に返される :class:`Symfony\\Component\\HttpFoundation\\File\\UploadedFile` オブジェクトを使用します。このクラスを使用することでアドバンテージを享受できます。

::

    public function upload()
    {
        // フィールドが必須でなければ、ファイルプロパティが空でも受け付けます
        if (null === $this->file) {
            return;
        }

        // ここではオリジナルの名前を使用します
        // しかし、セキュリティの対処のため、サニタイズはしてください
        
        // move メソッドは、対象となるディレクトリを受け取り、ファイルを移動します
        $this->file->move($this->getUploadRootDir(), $this->file->getClientOriginalName());

        // パスのプロパティには、ファイルの保存先をセットします
        $this->setPath($this->file->getClientOriginalName());

        // もう必要無いので、ファイルのプロパティを片付けます
        $this->file = null;
    }

ライフサイクルコールバックの使用
--------------------------------

実際にこの実装で動作はしますが、まだまだやることがあります。エンティティを保存する際に問題が起きたときのことを考えましょう。 ``path`` プロパティが正しく保存されていなくても、ファイルが既に最終的に保存する場所へ移動してしまっています。

この問題を回避するために、データベースの操作とファイル移動をアトミックになるような実装を変更する必要があります。エンティティの保存時やファイル移動時に問題が起きた際に、どちらかだけ処理されるのではなく、 *両方とも* 処理されてはいけないのです。

このため、データベースへエンティティの保存をするとすぐにファイルを移動するように変更してください。ライフサイクルコールバックのフックを使用すれば、それが可能になります。

::

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
    }

次に、 ``Document`` クラスをリファクタリングして、これらのコールバックのアドバンテージを受けることができるようにしてください。

::

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
        /**
         * @ORM\PrePersist()
         * @ORM\PreUpdate()
         */
        public function preUpload()
        {
            if (null !== $this->file) {
                // ユニークな名前を生成できれば、何でも構いません
                $this->setPath(uniqid().'.'.$this->file->guessExtension());
            }
        }

        /**
         * @ORM\PostPersist()
         * @ORM\PostUpdate()
         */
        public function upload()
        {
            if (null === $this->file) {
                return;
            }

            // ファイルを移動できなければ、ここで例外を投げてくだだい。
            // 例外が検知されれば、UploadFile の move() メソッドが行うデータベースの保存をしません。
            $this->file->move($this->getUploadRootDir(), $this->path);

            unset($this->file);
        }

        /**
         * @ORM\PostRemove()
         */
        public function removeUpload()
        {
            if ($file = $this->getAbsolutePath()) {
                unlink($file);
            }
        }
    }

これでこのクラスは、必要なことを全て実装しました。保存前にユニークな名前を生成して、保存後にファイルを移動、そしてエンティティが削除されたらファイルも削除する、と。

.. note::

    ``@ORM\PrePersist()`` と ``@ORM\PostPersist()`` のイベントコールバックは、それぞれエンティティのデーターベースへの保存前と保存後にトリガーします。一方、 ``@ORM\PreUpdate()`` と ``@ORM\PostUpdate()`` イベントコールバックは、エンティティの変更前と変更後にトリガーします。

.. caution::

    ``PreUpdate`` と ``PostUpdate`` のコールバックは、エンティティのフィールドのどれかが変更があって保存がされるときのみトリガーされます。つまり、デフォルトでは、 ``$file`` プロパティのみの変更であれば、このプロパティ自体は Doctrine を通して直接保存されるわけではないので、これらのイベントはトリガーされません。この問題の解決方法の１つとして、 Doctrine に保存する ``updated`` フィールドを使用して、ファイルに変更があった際にこのフィールドを変更して、トリガーをすることができます。

ファイル名として ``id`` を使用する
----------------------------------

ファイル名に ``id`` を使用したければ、実際の名前ではなく、 ``path`` プロパティの下に拡張子を保存する必要があるので、実装は多少異なります。

::

    use Symfony\Component\HttpFoundation\File\UploadedFile;

    /**
     * @ORM\Entity
     * @ORM\HasLifecycleCallbacks
     */
    class Document
    {
        /**
         * @ORM\PrePersist()
         * @ORM\PreUpdate()
         */
        public function preUpload()
        {
            if (null !== $this->file) {
                $this->setPath($this->file->guessExtension());
            }
        }

        /**
         * @ORM\PostPersist()
         * @ORM\PostUpdate()
         */
        public function upload()
        {
            if (null === $this->file) {
                return;
            }

            // ファイルを移動できなければ、ここで例外を投げてくだだい。
            // 例外が検知されれば、UploadFile の move() メソッドが行うデータベースの保存をしません。
            $this->file->move($this->getUploadRootDir(), $this->id.'.'.$this->file->guessExtension());

            unset($this->file);
        }

        /**
         * @ORM\PostRemove()
         */
        public function removeUpload()
        {
            if ($file = $this->getAbsolutePath()) {
                unlink($file);
            }
        }

        public function getAbsolutePath()
        {
            return null === $this->path ? null : $this->getUploadRootDir().'/'.$this->id.'.'.$this->path;
        }
    }


.. 2011/11/01 ganchiku 4ff78e97ea813537be372e49540d0e7a3ba41cac
